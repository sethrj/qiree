# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

from pyqir import BasicQisBuilder, SimpleModule, Value, PointerType, IntType, Constant, const
from typing import List
import pyqir.rt as rt


def if_else(qis: BasicQisBuilder, qubits: List[Value], results: List[Value]) -> None:
    msg = qubits[0]
    target = qubits[1]
    register = qubits[2]

    # Create some entanglement that we can use to send our message.
    qis.h(register)
    qis.cx(register, target)

    # Encode the message into the entangled pair.
    qis.cx(msg, register)
    qis.h(msg)

    # Measure the qubits to extract the classical data we need to decode the
    # message by applying the corrections on the target qubit accordingly.
    qis.mz(msg, results[0])
    qis.reset(msg)
    qis.if_result(results[0], one=lambda: qis.z(target))

    qis.mz(target, results[1])
    qis.reset(target)
    qis.if_result(results[0], one=lambda: target)

    qis.mz(register, results[2])
    qis.reset(register)
    qis.if_result(results[2], one=lambda: qis.x(target))


module = SimpleModule("if_else", num_qubits=3, num_results=3)
qis = BasicQisBuilder(module.builder)
if_else(qis, module.qubits, module.results)

i8p = PointerType(IntType(module.context, 8))

rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 3), Constant.null(i8p)
)

rt.result_record_output(module.builder, module.results[0], Constant.null(i8p))
rt.result_record_output(module.builder, module.results[1], Constant.null(i8p))
rt.result_record_output(module.builder, module.results[2], Constant.null(i8p))

if __name__ == "__main__":
    print(module.ir())
