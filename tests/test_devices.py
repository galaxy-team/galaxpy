import unittest
from galaxpy import saturn

class Test_Devices(unittest.TestCase):
    def setUp(self):
        self.cpu = saturn.dcpu()

        class test_device(saturn.device):
            def interrupt(self):
                print("Interrupted")
            def cycle(self):
                print("Cycled")
        self.device = test_device()
 
    def test_attach(self):
        self.cpu.attach_device(self.device)
        self.cpu.reset()

    def tearDown(self):
        self.cpu.reset()
        del self.device

    def test_metadata_assignment(self):
        self.device.id = 0x5555
        self.assertEqual(
            self.device.id,
            0x5555)
        
        self.device.manufacturer = 0x313373
        self.assertEqual(
            self.device.manufacturer,
            0x313373)
       
        self.device.name = "Flux Capacitor"
        self.assertEqual(
            self.device.name,
            "Flux Capacitor")

        self.device.version = 0x300
        self.assertEqual(
            self.device.version,
            0x300)

def main():
    unittest.main()

if __name__ == "__main__":
    main()
