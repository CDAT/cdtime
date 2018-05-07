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

    def testCdtimeCmp(self):
        res = cdtime.comptime(2000)<cdtime.comptime(2001)
        self.assertTrue(res)

        res = cdtime.comptime(2000).__lt__(cdtime.comptime(2001))
        self.assertTrue(res)

        res =cdtime.reltime(0,'hours since 2000')>cdtime.reltime(1,'hours since 2000')
        self.assertFalse(res)

        res = cdtime.reltime(0,'hours since 2000')<cdtime.reltime(1,'hours since 2000')
        self.assertTrue(res)

        res = cdtime.comptime(2000)>cdtime.comptime(2001)
        self.assertFalse(res)

        res = cdtime.comptime(2000)<cdtime.comptime(2001)
        self.assertTrue(res)

        res = cdtime.reltime(0,'years since 2000')>cdtime.reltime(1,'years since 2000')
        self.assertFalse(res)

        res = cdtime.reltime(0,'years since 2000')<cdtime.reltime(1,'years since 2000')
        self.assertTrue(res)

        res = cdtime.comptime(2000,1,1,0)>cdtime.comptime(2000,1,1,1)
        self.assertFalse(res)

        res = cdtime.comptime(2000,1,1,0)<cdtime.comptime(2000,1,1,1)
        self.assertTrue(res)

if __name__ == '__main__':
    unittest.main()

