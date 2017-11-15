import unittest
import cdtime

class CDTIMETest(unittest.TestCase):

    def testWrongUnits(self):
        try:
            b = cdtime.reltime(4, "days since YYYY-XX")
        except Exception as inst:
            self.assertEqual(inst.args[0], "Invalid relative time units")

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

if __name__ == '__main__':
    unittest.main()

