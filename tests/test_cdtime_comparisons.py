import unittest
import cdtime

class CDTimeComparisons(unittest.TestCase):
    def setUp(self):
        self.smallComp = cdtime.comptime(2000)
        self.bigComp = cdtime.comptime(2020)
        self.smallRel = cdtime.reltime(0,"days since 2000")
        self.bigRel = cdtime.reltime(0,"days since 2020")

    def testConvertc2r(self):
        self.assertEqual(cdtime.c2r(self.bigComp,"days since 2020"), self.bigRel)

    def testCmps2c(self):
        self.assertEqual(cdtime.s2c("2020-1-1"), self.bigRel)

    def testCmps2r(self):
        self.assertEqual(cdtime.s2r("2020-1-1","days since 2020"), self.bigRel)

    def testConvert2c(self):
        self.assertEqual(cdtime.c2r(self.bigComp,"days since 2020"), self.bigRel)

    def testConvert2r(self):
        self.assertEqual(cdtime.r2r(self.bigRel,"days since 2020"), self.bigRel)

    def testCmpComp(self):
        self.assertEqual(self.smallComp.cmp(self.bigComp),-2)
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
        #bigTime = cdtime.comptime(3000)
        #smallTime= cdtime.reltime(2,"{} since 2000".format('years'))
        #pdb.set_trace()
        #self.assertTrue(smallTime<bigTime)

if __name__ == '__main__':
   unittest.main()

