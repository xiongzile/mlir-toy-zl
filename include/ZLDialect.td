#ifndef ZL_DIALECT
#define ZL_DIALECT
include "mlir/IR/OpBase.td"
// Provide a definition of the 'zl' dialect in the ODS framework so that we
// can define our operations.
def Zl_Dialect : Dialect {
  // The namespace of our dialect, this corresponds 1-1 with the string we
  // provided in `ToyDialect::getDialectNamespace`.
  let name = "zl";

  // A short one-line summary of our dialect.
  let summary = "A high-level dialect for analyzing and optimizing the "
                "Toy language";

  // A much longer description of our dialect.
  let description = [{
    The Toy language is a tensor-based language that allows you to define
    functions, perform some math computation, and print results. This dialect
    provides a representation of the language that is amenable to analysis and
    optimization.
  }];

  // The C++ namespace that the dialect class definition resides in.
  let cppNamespace = "::zl";
}

// Base class for zl dialect operations. This operation inherits from the base
// `Op` class in OpBase.td, and provides:
//   * The parent dialect of the operation.
//   * The mnemonic for the operation, or the name without the dialect prefix.
//   * A list of traits for the operation.
class Zl_OP<string mnemonic, list<Trait> traits = []>
    : Op<Zl_Dialect, mnemonic, traits>;

def ConstantOp : Zl_OP<"constant"> {
  // Provide a summary and description for this operation. This can be used to
  // auto-generate documentation of the operations within our dialect.
  let summary = "constant operation";
  let description = [{
    Constant operation turns a literal into an SSA value. The data is attached
    to the operation as an attribute. For example:

      %0 = "zl.constant"()
         { value = dense<[[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]]> : tensor<2x3xf64> }
        : () -> tensor<2x3xf64>
  }];

  // The constant operation takes an attribute as the only input.
  // `F64ElementsAttr` corresponds to a 64-bit floating-point ElementsAttr.
  let arguments = (ins F64ElementsAttr:$value);

  // The generic call operation returns a single value of TensorType.
  // F64Tensor corresponds to a 64-bit floating-point TensorType.
  let results = (outs F64Tensor);

  // Add additional verification logic to the constant operation. Setting this
  // bit to `1` will generate a `::llvm::LogicalResult verify()` declaration on
  // the operation class that is called after ODS constructs have been verified,
  // for example the types of arguments and results. We implement additional
  // verification in the definition of this `verify` method in the C++ source
  // file.
  let hasVerifier = 1;
}

def PrintOp : Zl_OP<"print"> {
  let summary = "print operation";
  let description = [{
        print the information of a tensor
    }];

  let arguments = (ins F64Tensor:$value);
}
#endif
