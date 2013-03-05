import sys

# unit testing specific imports
import unittest

# unit test subunits
import tests.test_saturn
#import tests.test_jupiter


def main():
    loader = unittest.TestLoader()
    suite = loader.loadTestsFromModule(tests.test_saturn)
#    suite.addTests(loader.loadTestsFromModule(tests.test_jupiter))
    runner = unittest.TextTestRunner(verbosity=2)
    
    end = runner.run(suite)
    if len(end.errors) > 1:
        print('{} errors appear to have occured.'.format(len(end.errors)))
        print('Informing the test environment of such')
        sys.exit(1)

if __name__ == '__main__':
    main()

