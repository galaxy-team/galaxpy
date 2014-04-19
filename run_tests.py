import sys

# unit testing specific imports
import unittest

names = [
    'test_saturn',
    'test_devices',
    'test_jupiter',
    'test_pluto'
]
import tests


def main():
    loader = unittest.TestLoader()
    suite = loader.loadTestsFromNames(names, module=tests)
    runner = unittest.TextTestRunner(verbosity=2)

    end = runner.run(suite)
    if len(end.errors) > 1:
        print('{} errors appear to have occured.'.format(len(end.errors)))
        print('Informing the test environment of such')
        sys.exit(1)

if __name__ == '__main__':
    main()
