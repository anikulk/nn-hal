#include <Add.hpp>

namespace android {
namespace hardware {
namespace neuralnetworks {
namespace nnhal {

Add::Add(int operationIndex) : OperationsBase(operationIndex) {
    mDefaultOutputIndex = sModelInfo->getOperationOutput(mNnapiOperationIndex, 0);
}

bool Add::validate() {
    // check output type
    if (!checkOutputOperandType(0, (int32_t)OperandType::TENSOR_FLOAT32)) {
        return false;
    }

    // Check all input types
    for (int i = 0; i <= 1; i++) {
        if (!checkInputOperandType(i, (int32_t)OperandType::TENSOR_FLOAT32)) return false;
    }

    if (!checkInputOperandType(2, (int32_t)OperandType::INT32)) {
        return false;
    }

    return true;
}

std::shared_ptr<ngraph::Node> Add::createNode() {
    // Creating input nodes
    auto input1 = getInputNode<float>(0);
    auto input2 = getInputNode<float>(1);

    auto activationFn = sModelInfo->ParseOperationInput<uint32_t>(mNnapiOperationIndex, 2);

    auto addNode =
        std::make_shared<ngraph::opset3::Add>(input1, input2, ngraph::op::AutoBroadcastType::NUMPY);

    auto outputNode = applyActivation(addNode, activationFn);

    const auto op = sModelInfo->getOperand(mDefaultOutputIndex);
    if (op.lifetime == V1_3::OperandLifeTime::SUBGRAPH_OUTPUT) {
        addResultNode(mDefaultOutputIndex, outputNode);
    }
    return outputNode;
}

std::shared_ptr<ngraph::Node> Add::createNodeForPlugin() {
    if (sPluginType == "VPU") {
        auto input = mNgraphNodes->getOperationOutput(
            sModelInfo->getOperationInput(mNnapiOperationIndex, OP_INPUT_IDX_CONV));
        std::shared_ptr<ngraph::Node> constantOp =
            std::make_shared<ngraph::opset3::Constant>(ngraph::element::f32, input.get_shape());
        auto transposedOp = transpose(NHWC_NCHW, constantOp);
        return std::make_shared<ngraph::opset3::Add>(input, transposedOp,
                                                     ngraph::op::AutoBroadcastType::NUMPY);
    } else {
        return createNode();
    }
}

}  // namespace nnhal
}  // namespace neuralnetworks
}  // namespace hardware
}  // namespace android
