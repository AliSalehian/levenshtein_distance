import unittest
import levenshtein

class TestLevenshteinDistance(unittest.TestCase):

    def test_levenshtein_strings(self):
        self.assertEqual(levenshtein.single_dist("hello", "hello"), 0, "Test 1 failed")
        self.assertEqual(levenshtein.single_dist("abc", "xyz"), 3, "Test 2 failed")
        self.assertEqual(levenshtein.single_dist("abcd", ""), 4, "Test 3 failed")
        self.assertEqual(levenshtein.single_dist("kitten", "sitting"), 3, "Test 4 failed")
        self.assertEqual(levenshtein.single_dist("", ""), 0, "Test 5 failed")
        self.assertEqual(levenshtein.single_dist("a", "b"), 1, "Test 6 failed")
        self.assertEqual(levenshtein.single_dist("flaw", "flaws"), 1, "Test 7 failed")
        self.assertEqual(levenshtein.single_dist("abc", "ABC"), 3, "Test 8 failed")
        self.assertEqual(levenshtein.single_dist("abcd", "dcba"), 4, "Test 9 failed")
        self.assertEqual(levenshtein.single_dist("aaaa", "aa"), 2, "Test 10 failed")
        self.assertEqual(levenshtein.single_dist("intention", "execution"), 5, "Test 11 failed")
        self.assertEqual(levenshtein.single_dist("abcdefghijklmnopqrstuvwxyz", "zyxwvutsrqponmlkjihgfedcba"), 26, "Test 12 failed")

    def test_levenshtein_lists(self):
        self.assertEqual(levenshtein.single_dist([32, 39, 53], [21, 36, 52, 25, 53]), 4, "Test 13 failed")
        self.assertEqual(levenshtein.single_dist([34, 56, 55, 25, 36], [34, 56, 55, 25, 36]), 0, "Test 14 failed")
        self.assertEqual(levenshtein.single_dist([28, 33, 28, 51], [28, 33, 28]), 1, "Test 15 failed")

if __name__ == "__main__":
    unittest.main()
