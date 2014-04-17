from __future__ import print_function
import IMP.test
import os.path
import sys
from io import BytesIO, StringIO


class Tests(IMP.test.TestCase):

    def test_data_directory(self):
        """Test that conversions to input files work properly"""
        print("starting")
        IMP.set_log_level(IMP.MEMORY)
        v = IMP._test_ifile(self.get_input_file_name("text"))
        self.assertEqual(v, "word")
        v = IMP._test_ifile(self.open_input_file("text"))
        self.assertEqual(v, "word")
        self.assertRaises(IOError, IMP._test_ifile, "notafile")
        s = BytesIO(b"hi there")
        v = IMP._test_ifile(s)
        self.assertEqual(v, "hithere")
        print("done")

    def test_odata_directory(self):
        """Test that conversions to output files work properly"""
        print("starting")
        IMP.set_log_level(IMP.MEMORY)
        IMP._test_ofile("ofile_test")
        self.assertRaises(IOError, IMP._test_ofile, "nodir/hi")
        f = open("hi", "w")
        IMP._test_ofile(f)
        # In Python 3 binary files are handled differently (as raw bytes,
        # not Unicode)
        f = open("hi", "wb")
        IMP._test_ofile(f)
        s = BytesIO()
        IMP._test_ofile(s)
        self.assertTrue(s.getvalue().startswith(b"hi\n"))
        if sys.version_info[0] >= 3:
            s = StringIO()
            IMP._test_ofile(s)
            self.assertTrue(s.getvalue().startswith("hi\n"))
        self.assertRaises(TypeError, IMP._test_ofile, 1)

        class NoMethods(object):
            pass
        self.assertRaises(TypeError, IMP._test_ofile, NoMethods)
        del f
        print("unlinking")
        # os.unlink('ofile_test')
        # os.unlink('hi')
        print("done")

    def test_odata_directory_2(self):
        """Test temporary files"""
        simple = IMP.create_temporary_file()
        print(simple.get_name())
        prefix = IMP.create_temporary_file("imp")
        print(prefix.get_name())
        self.assertIn("imp", prefix.get_name())
        suffix = IMP.create_temporary_file("imp", ".py")
        print(suffix.get_name())
        self.assertIn("imp", suffix.get_name())
        self.assertIn(".py", suffix.get_name())

    def test_bad(self):
        """Test bad paths trigger IO exceptions with overloads"""
        self.assertRaises(
            IOError,
            IMP._test_ifile_overloaded,
            "bad path",
            "hi")
        s = BytesIO()
        # shouldn't raise
        IMP._test_ifile_overloaded(s, "hi")
if __name__ == '__main__':
    IMP.test.main()