#!/usr/bin/python3-dbg

# Example:
# Two asteroids:
# asteroid0: (was ".EXPORT start \ start: SET PC, start")
    # object_code: 0x7f81 0x0000
    # imported_labels: {}
    # exported_labels: {"start": 0x0000}
    # used_labels: {0x0000}
# asteroid1: (was ".IMPORT start \ func: SET A, 5 \ SET PC, start"
    # object_code: 0x7c01 0x0005 0x7f81 0x0000
    # imported_labels: {3: "start"}
    # exported_labels: {}
    # used_labels: {} (no labels local to the file used)

# Step 1: Work out the offsets for each asteroid
    # By random chance, asteroid1 will be offset 0,
    # asteroid0 will be offset 4 (the length of asteroid1)
# Step 2: Shift their used_labels
    # asteroid0.object_code: 0x7f81 0x0002
    # asteroid1.object_code: 0x7c01 0x0005 0x7f81 0x0000
# Step 3: Create a global exported_labels map.
    # {"start": 0x0004} (offset by 4)
# Steps 3a & 4: All good
# Step 5: Shift their imported_labels
    # asteroid0.object_code: 0x7f81 0x0002
    # asteroid1.object_code: 0x7c01 0x0005 0x7f81 0x0004
# Step 6: Concatenate together final binaries
    # 0x7c01 0x0005 0x7f81 0x0004 0x7f81 0x0002

# from galaxpy import jupiter
# asteroid0 = jupiter.assemble(
#     ".EXPORT start\n"
#     ":start\n"
#     "SET PC, start"
# )


import unittest

from galaxpy import pluto
from galaxpy.asteroid import Asteroid


class TestPlutoUnittest(unittest.TestCase):
    def test_basic(self):

        asteroid0 = Asteroid(
            object_code={0x7f81, 0x0000},
            imported_labels={},
            exported_labels={"start": 0x0000},
            used_labels={0x0000}
        )

        asteroid1 = Asteroid(
            object_code={0x7c01, 0x0005, 0x7f81, 0x0000},
            imported_labels={3: "start"},
            exported_labels={},
            used_labels={}
        )

        objects = [
            asteroid0,
            asteroid1
        ]

        binary = pluto.link(objects)

        self.assertListEqual(
            binary,
            [0x7c01, 0x0005, 0x7f81, 0x0004, 0x7f81, 0x0002]
        )

    def test_really_basic(self):
        self.assertListEqual(pluto.link([Asteroid()]), [])

        self.assertListEqual(
            pluto.link([Asteroid(object_code=[0x0])]),
            [0x0]
        )
