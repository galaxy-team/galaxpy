import unittest
from galaxpy import saturn

class Test_Saturn(unittest.TestCase):
    def setUp(self):
        self.cpu = saturn.dcpu()

    def test_default_values(self):
        self.assertEqual(self.cpu.A, 0)
        self.assertEqual(self.cpu.B, 0)
        self.assertEqual(self.cpu.C, 0)

#        self.assertEqual()

        self.cpu.reset()
    
    def test_flash(self):
        opcodes = [0x7c01, 0x0f00, 0x7fc1, 0x0001, 0xdead, 0x03c2, 0xdead]

        self.cpu.flash(opcodes)

        self.assertTrue(list(self.cpu)[:len(opcodes)] == opcodes)

        self.cpu.reset()
        

def main():
    unittest2.main()

if __name__ == "__main__":
    main()
