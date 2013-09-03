# NOTE: This file is generated from modules/core/tools/container_tempates/test.py
# do not edit.

import IMP
import IMP.test
import IMP.core
import IMP.container
import math


# This file is generated by the make-container script

class Tests(IMP.test.TestCase):
    """Tests for PairContainer related objects"""


    def test_restraint(self):
        """Simple test of the PairsRestraint"""
        m= IMP.kernel.Model()
        cps= IMP.kernel._ConstPairScore(1)
        ps= [IMP.kernel.Particle(m) for i in range(0,10)]
        psc= IMP.container.ListSingletonContainer(ps)
        apc= IMP.container.AllPairContainer(psc)
        r= IMP.container.PairsRestraint(cps, apc)
        self.assertEqual(r.evaluate(False), apc.get_number_of_particle_pairs())

        lpc= IMP.container.ListPairContainer(apc.get_particle_pairs())
        r2= IMP.container.PairsRestraint(cps, lpc)
        self.assertEqual(r2.evaluate(False), apc.get_number_of_particle_pairs())
        self.assertEqual(r2.evaluate(False), apc.get_number_of_particle_pairs())
        m.add_restraint(r2)
        self.assertEqual(m.evaluate(False), apc.get_number_of_particle_pairs())
        self.assertEqual(m.evaluate(False), apc.get_number_of_particle_pairs())

if __name__ == '__main__':
    IMP.test.main()
