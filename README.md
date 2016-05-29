# python_gil
A sample code to reproduce the question on SO.
http://stackoverflow.com/questions/37513957/does-python-gil-need-to-be-taken-care-when-work-with-multi-thread-c-extension.

To compile the proc module, use

python setupProc.py build

To compile the pub module, use

python setupPub.py build

To run the code, use

import main
main.main()
