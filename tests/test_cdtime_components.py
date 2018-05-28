import unittest
import cdtime

class CDTIMETest(unittest.TestCase):
    def less(self, a, b):
        print("Testing",a,b,"for less")
        self.assertTrue(a<b)
        self.assertTrue(a.__lt__(b))
        self.assertFalse(b<a)
        self.assertFalse(b.__lt__(a))
        self.assertFalse(a<a)
        self.assertFalse(a.__lt__(a))
    def greater(self, a, b):
        print("Testing",a,b,"for greater")
        self.assertTrue(a>b)
        self.assertTrue(a.__gt__(b))
        self.assertFalse(b>a)
        self.assertFalse(b.__gt__(a))
        self.assertFalse(a>a)
        self.assertFalse(a.__gt__(a))
    def less_equal(self, a, b):
        print("Testing",a,b,"for less equal")
        self.assertTrue(a<=b)
        self.assertTrue(a.__le__(b))
        self.assertFalse(b<=a)
        self.assertFalse(b.__le__(a))
        self.assertTrue(a<=a)
        self.assertTrue(a.__le__(a))
    def greater_equal(self, a, b):
        print("Testing",a,b,"for greater equal")
        self.assertTrue(a>=b)
        self.assertTrue(a.__ge__(b))
        self.assertFalse(b>=a)
        self.assertFalse(b.__ge__(a))
        self.assertTrue(a>=a)
        self.assertTrue(a.__ge__(a))

    def equal(self, a, b):
        print("Testing",a,b,"for equal")
        self.assertTrue(a==b)
        self.assertTrue(a.__eq__(b))
        self.assertFalse(a!=b)
        self.assertFalse(a.__ne__(b))

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

    def setup(self):

        # Component Time
        a = cdtime.comptime(2000)
        b = cdtime.comptime(2001)
        self.less(a,b)
        self.less_equal(a,b)
        self.greater(b,a)
        self.greater_equal(b,a)
        self.equal(a,a)

    def testCdtimeRelCmp(self):
        # Reltime
        for units in ["years","months","days","minutes","seconds"]:
            a = cdtime.reltime(1,"{} since 2000".format(units))
            b = cdtime.reltime(2,"{} since 2000".format(units))
            self.less(a,b)
            self.less_equal(a,b)
            self.greater(b,a)
            self.greater_equal(b,a)
            self.equal(a,a)
            
    #
    # These test are not allowed in python 2 
    # class addresses cannot be compared this way.
    #
    def dtestCdtimeCompRelLTCmp(self):
        # Comp and rel
        a = cdtime.comptime(2000)
        for units in ["years","months","days","minutes","seconds"]:
            b = cdtime.reltime(2,"{} since 2000".format(units))
            self.less(a,b)
            self.less_equal(a,b)
            self.greater(b,a)
            self.greater_equal(b,a)
            self.equal(a,a)

    def dtestCdtimeCompRelGTCmp(self):
        # Rel and comp
        b = cdtime.comptime(3000)
        for units in ["years","months","days","minutes","seconds"]:
            a = cdtime.reltime(2,"{} since 2000".format(units))
            self.less(a,b)
            self.less_equal(a,b)
            self.greater(b,a)
            self.greater_equal(b,a)
            self.equal(a,a)
    # Comp and rel
    def dtestCdtimeLTCompCmp(self):
        res = cdtime.comptime(2000)<cdtime.reltime(2,"days since 2000")
        self.assertTrue(res)

    def dtestCdtimeGTCompCmp(self):
        res = cdtime.comptime(2000)>cdtime.reltime(2,"days since 1990")
        self.assertTrue(res)

    def dtestCdtimeEQCompCmp(self):
        res = cdtime.comptime(2000)==cdtime.reltime(0,"days since 2000")
        self.assertTrue(res)

    def dtestCdtimeGTRelCmp(self):
        res = cdtime.reltime(2,"days since 2000")>cdtime.comptime(2000)
        self.assertTrue(res)

    def dtestCdtimeLTRelCmp(self):
        res = cdtime.reltime(2,"days since 2000")<cdtime.comptime(2000,1,4)
        self.assertTrue(res)

    def dtestCdtimeEQRelCmp(self):
        res = cdtime.reltime(2,"days since 2000") == cdtime.comptime(2000,1,3)
        self.assertTrue(res)

if __name__ == '__main__':
    unittest.main()

