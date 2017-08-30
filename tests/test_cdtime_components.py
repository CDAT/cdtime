import unittest
import cdtime

class CDTIMETest(unittest.TestCase):
    def testCdtimeComponent(self):
        tc = cdtime.comptime(2017)
        value = 4
        tc.month = value
        tc.year = value
        tc.day = value
        tc.hour = value
        tc.minute = value
        tc.second = value
        value = 4.
        tc.second = value

