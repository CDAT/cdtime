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
    def testOperatorRel(self):
        self.assertEqual(self.smallRel,self.smallRel)
#        self.assertTrue(self.smallRel==self.smallComp)
        self.assertLess(self.smallRel,self.bigRel)
        self.assertLess(self.smallRel,self.bigComp)
        self.assertGreater(self.bigRel,self.smallRel)
        self.assertGreater(self.bigRel,self.smallComp)
    def testOperatorComp(self):
        self.assertTrue(self.smallComp==self.smallRel)
#        self.assertEqual(self.smallComp,self.smallComp)
        self.assertLess(self.smallComp,self.bigRel)
        self.assertLess(self.smallComp,self.bigComp)
        self.assertGreater(self.bigComp,self.smallRel)
        self.assertGreater(self.bigComp,self.smallComp)

if __name__ == '__main__':
   unittest.main()

