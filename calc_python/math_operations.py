# This Python file uses the following encoding: utf-8
class MathOperations:
    def Add(self, a, b):
        return a + b

    def Substract(self, a, b):
        return a - b

    def Multiply(self, a, b):
        return a * b

    def Divide(self, a, b):
        if b == 0:
            return float('nan')
        return a / b

    def __init__(self):
        pass
