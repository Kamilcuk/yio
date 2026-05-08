import unittest
import sys
from pathlib import Path

# Add src to sys.path to import preprocess
sys.path.insert(0, str(Path(__file__).parent.parent.parent / "src"))

from preprocess import _convert_raw, j_FLOATS, j_FLOATREPRS

class TestPreprocess(unittest.TestCase):
    def test_convert_raw(self):
        raw = [["key1", "key2"], ["val1", "val2"]]
        result = _convert_raw(raw)
        self.assertEqual(len(result), 1)
        self.assertEqual(result[0]["key1"], "val1")
        self.assertEqual(result[0][1], "val1")
        self.assertEqual(result[0]["key2"], "val2")
        self.assertEqual(result[0][2], "val2")

    def test_j_floats(self):
        # Find float 'f'
        f = next(x for x in j_FLOATS if x['N'] == 'f')
        self.assertEqual(f['type'], 'float')
        self.assertEqual(f['math'], 'f')
        self.assertIn('B32', f['reprs'])

        # Find double 'd'
        d = next(x for x in j_FLOATS if x['N'] == 'd')
        self.assertEqual(d['type'], 'double')
        self.assertEqual(d['math'], '')

    def test_j_floatreprs(self):
        # Find B64
        b64 = next(x for x in j_FLOATREPRS if x['name'] == 'B64')
        self.assertEqual(b64['mant'], 53)

        # Find D64
        d64 = next(x for x in j_FLOATREPRS if x['name'] == 'D64')
        self.assertEqual(d64['mant'], 16)

    def test_no_header_in_data(self):
        # Header for j_FLOATS is ["N", "G", "type", "math", "strto", "reprs"]
        for entry in j_FLOATS:
            self.assertNotEqual(entry['N'], 'N')
            self.assertNotEqual(entry['type'], 'type')

        # Header for j_FLOATREPRS is ["name", "mant"]
        for entry in j_FLOATREPRS:
            self.assertNotEqual(entry['name'], 'name')
            self.assertNotEqual(entry['mant'], 'mant')

    def test_dual_indexing(self):
        # Check j_FLOATS: header is ["N", "G", "type", "math", "strto", "reprs"]
        header_floats = ["N", "G", "type", "math", "strto", "reprs"]
        for entry in j_FLOATS:
            for i, key in enumerate(header_floats):
                # Check that j_FLOATS[x][i+1] == j_FLOATS[x][key]
                self.assertEqual(entry[i + 1], entry[key], f"Mismatch at index {i+1} (key '{key}') for entry {entry['N']}")

        # Check j_FLOATREPRS: header is ["name", "mant"]
        header_reprs = ["name", "mant"]
        for entry in j_FLOATREPRS:
            for i, key in enumerate(header_reprs):
                # Check that j_FLOATREPRS[x][i+1] == j_FLOATREPRS[x][key]
                self.assertEqual(entry[i + 1], entry[key], f"Mismatch at index {i+1} (key '{key}') for entry {entry['name']}")

    def test_header_assertion(self):
        # Digit-only header should fail
        raw = [["1", "key2"], ["val1", "val2"]]
        with self.assertRaises(AssertionError):
            _convert_raw(raw)

        # Non-digit header should pass
        raw = [["key1", "key2"], ["val1", "val2"]]
        _convert_raw(raw)

if __name__ == '__main__':
    unittest.main()
