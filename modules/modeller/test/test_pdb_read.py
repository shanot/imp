import unittest
import IMP
import IMP.test
import IMP.modeller
import IMP.core

class PDBReadTest(IMP.test.TestCase):
    #def test_open_error(self):
    #    """Check exception thrown on nonexistant file"""
    #    m=IMP.Model()
    #    self.assertRaises(IMP.IOError,  read_pdb, 'fake_file.pdb', m)
    def test_hierarchy(self):
        """Check reading a pdb with one protein"""
        #i_num_res_type= IMP.atom.ResidueType.get_number_unique()
        #i_num_atom_type= IMP.atom.AtomType.get_number_unique()
        m = IMP.Model()
        mp= IMP.modeller.read_pdb(
                       self.get_input_file_name('single_protein.pdb'), m)
        #mp= IMP.core.Hierarchy.decorate_particle(p)
        #mp.show()
        #IMP.core.show_molecular_hierarchy(mp)
        mp.validate()
        hc= IMP.core.HierarchyCounter()
        IMP.core.depth_first_traversal(mp, hc)
        #f_num_res_type= IMP.atom.ResidueType.get_number_unique()
        #f_num_atom_type= IMP.atom.AtomType.get_number_unique()
        mpp= mp.get_parent()
        self.assertEqual(mpp, IMP.atom.Hierarchy(),
                         "Should not have a parent")
        mpc= mp.get_child(0)
        self.assertEqual(mpc.get_parent(), mp,
                         "Should not have a parent")
        #print str(hc.get_count())
        #self.assertEqual(i_num_res_type, f_num_res_type, "too many residue types")
        #self.assertEqual(i_num_atom_type, f_num_atom_type, "too many atom types")
        self.assertEqual(1377, hc.get_count(),
                         "Wrong number of particles created")
        rd= IMP.atom.Residue(IMP.atom.get_residue(mp, 29).get_particle())
        at= IMP.atom.get_atom(rd, IMP.atom.AtomType("C"))
        self.assertEqual(IMP.atom.get_residue(at).get_index(), rd.get_index())
        self.assertEqual(rd.get_index(), 29)

    def test_bonds(self):
        """Check that the file loader produces bonds"""
        m = IMP.Model()
        mp= IMP.modeller.read_pdb(
                      self.get_input_file_name('single_protein.pdb'), m)
        #mp= IMP.core.Hierarchy.decorate_particle(p)
        all_atoms= IMP.atom.get_by_type(mp,
                             IMP.atom.Hierarchy.ATOM)
        self.assertEqual(1221, len(all_atoms),
                         "Wrong number of atoms found in protein")

    def test_dna(self):
        """Check reading a dna with one chain"""
        def na_patches(mdl):
            """Nucleic acid terminal patches."""
            mdl.patch('5TER', mdl.residues[0])
            mdl.patch('3TER', mdl.residues[-1])
        m = IMP.Model()
        mp= IMP.modeller.read_pdb(self.get_input_file_name('single_dna.pdb'),
                                  m, na_patches)
        print "done reading"
        #IMP.core.show_molecular_hierarchy(mp)
        #mp= IMP.core.Hierarchy.decorate_particle(p)
        #mp.show()
        #IMP.core.show_molecular_hierarchy(mp)
        mp.validate()
        hc= IMP.core.HierarchyCounter()
        IMP.core.depth_first_traversal(mp, hc)
        mpp= mp.get_parent()
        self.assertEqual(mpp, IMP.atom.Hierarchy(),
                         "Should not have a parent")
        mpc= mp.get_child(0)
        self.assertEqual(mpc.get_parent(), mp,
                         "Should not have a parent")
        print str(hc.get_count())
        self.assertEqual(3160, hc.get_count(),
                         "Wrong number of particles created")
        #rd= IMP.get_residue(mp, 29)
        #self.assertEqual(rd.get_index(), 29)

if __name__ == '__main__':
    unittest.main()
