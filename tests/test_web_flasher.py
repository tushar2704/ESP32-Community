#!/usr/bin/env python3
"""
Unit and compatibility tests for ESP32 Community Web Flasher.
Validates JSON manifests, binary files, ESP32 magic headers, and flash offsets.
"""

import os
import json
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
WEB_FLASHER_DIR = REPO_ROOT / "web-flasher"

class TestWebFlasherCompatibility(unittest.TestCase):

    def setUp(self):
        self.manifests = list(WEB_FLASHER_DIR.glob("manifest-*.json"))
        self.assertTrue(len(self.manifests) >= 10, f"Expected at least 10 manifests, found {len(self.manifests)}")

    def test_manifests_valid_json(self):
        """Verify each manifest is valid JSON with required ESP Web Tools fields."""
        for manifest_path in self.manifests:
            with open(manifest_path, "r", encoding="utf-8") as f:
                try:
                    data = json.load(f)
                except Exception as e:
                    self.fail(f"Invalid JSON in {manifest_path.name}: {e}")

            self.assertIn("name", data, f"Missing 'name' in {manifest_path.name}")
            self.assertIn("version", data, f"Missing 'version' in {manifest_path.name}")
            self.assertIn("builds", data, f"Missing 'builds' in {manifest_path.name}")
            self.assertTrue(len(data["builds"]) > 0, f"Empty 'builds' in {manifest_path.name}")

    def test_binary_files_exist_and_not_empty(self):
        """Verify every binary referenced in every manifest exists and has non-zero size."""
        for manifest_path in self.manifests:
            with open(manifest_path, "r", encoding="utf-8") as f:
                data = json.load(f)

            for build in data["builds"]:
                self.assertIn("chipFamily", build)
                self.assertIn("parts", build)
                for part in build["parts"]:
                    rel_path = part["path"]
                    full_path = WEB_FLASHER_DIR / rel_path
                    self.assertTrue(full_path.exists(), f"Binary not found: {rel_path} in {manifest_path.name}")
                    size = full_path.stat().st_size
                    self.assertGreater(size, 0, f"Empty binary: {rel_path} in {manifest_path.name}")

    def test_esp32_binary_magic_byte(self):
        """Verify byte 0 of every firmware and bootloader is 0xE9 (ESP image magic byte)."""
        ESP_IMAGE_MAGIC = 0xE9

        for manifest_path in self.manifests:
            with open(manifest_path, "r", encoding="utf-8") as f:
                data = json.load(f)

            for build in data["builds"]:
                for part in build["parts"]:
                    rel_path = part["path"]
                    # boot_app0 is an OTA data sector, not an executable image
                    if "boot_app0" in rel_path or "partitions" in rel_path:
                        continue
                    full_path = WEB_FLASHER_DIR / rel_path
                    with open(full_path, "rb") as bf:
                        first_byte = bf.read(1)[0]
                    self.assertEqual(
                        first_byte, 
                        ESP_IMAGE_MAGIC, 
                        f"Corrupted or invalid ESP binary (magic byte 0x{first_byte:02X} != 0xE9) in {rel_path}"
                    )

    def test_flash_offsets(self):
        """Verify standard flash offsets match ESP-IDF and ESP Web Tools specifications."""
        for manifest_path in self.manifests:
            with open(manifest_path, "r", encoding="utf-8") as f:
                data = json.load(f)

            for build in data["builds"]:
                chip = build["chipFamily"]
                for part in build["parts"]:
                    path = part["path"]
                    offset = part["offset"]

                    if "bootloader" in path:
                        if chip == "ESP32":
                            self.assertEqual(offset, 4096, f"Classic ESP32 bootloader must be at 0x1000 in {path}")
                        else:
                            self.assertEqual(offset, 0, f"ESP32-S3/C3 bootloader must be at 0x0000 in {path}")
                    elif "partitions" in path:
                        self.assertEqual(offset, 32768, f"Partitions must be at 0x8000 in {path}")
                    elif "boot_app0" in path:
                        self.assertEqual(offset, 57344, f"boot_app0 must be at 0xe000 in {path}")
                    elif "firmware" in path:
                        self.assertEqual(offset, 65536, f"Firmware must be at 0x10000 in {path}")

    def test_index_html_references_all_manifests(self):
        """Verify web-flasher/index.html includes all generated manifests in its catalog."""
        index_file = WEB_FLASHER_DIR / "index.html"
        self.assertTrue(index_file.exists())
        index_content = index_file.read_text(encoding="utf-8")

        for manifest_path in self.manifests:
            self.assertIn(
                manifest_path.name,
                index_content,
                f"Manifest {manifest_path.name} not found in web-flasher/index.html catalog!"
            )

if __name__ == "__main__":
    unittest.main()
