import cdtime
import unittest
class TestCDTIME(unittest.TestCase):

   def testReltime(self):
        a = cdtime.reltime(28, "days since 1996-1-1")
        self.assertEqual(repr(a),'28.000000 days since 1996-1-1')

   def testRomptime(self):
        c = cdtime.comptime(1996, 2, 28)
        self.assertEqual(repr(c),"1996-2-28 0:0:0.0")
        newtime = c.torel("days since 1995-1-1")
        self.assertEqual(repr(newtime), '423.000000 days since 1995-1-1')

if __name__ == "__main__":
    unittest.main()

