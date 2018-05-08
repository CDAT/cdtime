import unittest
import cdtime

class CDTimeComparisons(unittest.TestCase):
    def setUp(self):
        self.smallComp = cdtime.comptime(2000)
        self.bigComp = cdtime.comptime(2020)
        self.smallRel = cdtime.reltime(0,"days since 2000")
        self.bigRel = cdtime.reltime(0,"days since 2020")

    def testCmpComp(self):
        self.assertEqual(self.smallComp.cmp(self.bigComp),-1)
        self.assertEqual(self.smallComp.cmp(self.bigRel),-1)
        self.assertEqual(self.smallComp.cmp(self.smallComp),0)
        self.assertEqual(self.smallComp.cmp(self.smallRel),0)
        self.assertEqual(self.bigComp.cmp(self.smallComp),1)
        self.assertEqual(self.bigComp.cmp(self.smallRel),1)
    def testCmpRel(self):
        self.assertEqual(self.smallRel.cmp(self.bigComp),-1)
        self.assertEqual(self.smallRel.cmp(self.bigRel),-1)
        self.assertEqual(self.smallRel.cmp(self.smallComp),0)
        self.assertEqual(self.smallRel.cmp(self.smallRel),0)
        self.assertEqual(self.bigRel.cmp(self.smallComp),1)
        self.assertEqual(self.bigRel.cmp(self.smallRel),1)
    def testConversion(self):
        bigTransComp = self.bigRel.tocomp()
        self.assertTrue(bigTransComp == self.bigComp)
        bigTransRel = self.bigComp.torel("days since 2020")
        self.assertTrue(bigTransRel == self.bigRel)

if __name__ == '__main__':
   unittest.main()

