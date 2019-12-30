import os
import sys
import cdat_info

test_suite_name = 'cdtime'

workdir = os.getcwd()
ret_code = SUCCESS
if __name__ == '__main__':
    runner = cdat_info.TestRunnerBase(test_suite_name)
    ret_code = runner.run(workdir)

sys.exit(ret_code)

