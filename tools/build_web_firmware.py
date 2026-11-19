#!/usr/bin/env python3
"""
ESP32 Community: Web Flasher Firmware Builder & Manifest Generator
Compiles flagship projects with PlatformIO, extracts binaries, and generates ESP Web Tools manifests.
"""

import os
import sys
import json
import shutil
import subprocess
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
WEB_FLASHER_DIR = REPO_ROOT / "web-flasher"
FIRMWARE_DEST_DIR = WEB_FLASHER_DIR / "firmware"

# Default PlatformIO executable path
PIO_BIN = shutil.which("pio") or str(Path.home() / "Library/Python/3.9/bin/pio")

PROJECTS = [
    {
        "slug": "hello-esp32",
        "name": "Hello ESP32 Hardware Diagnostic",
        "category": "Beginner",
        "dir": "projects/beginner/01-hello-esp32",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "wifi-scanner",
        "name": "2.4 GHz Wi-Fi Spectrum Scanner",
        "category": "Beginner",
        "dir": "projects/beginner/02-wifi-scanner",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "blink-rgb",
        "name": "Smooth Multi-Channel PWM RGB Cycle",
        "category": "Beginner",
        "dir": "projects/beginner/03-blink-rgb",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "web-server-hello",
        "name": "Embedded Web Server & IoT Hub",
        "category": "Beginner",
        "dir": "projects/beginner/04-web-server-hello",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "wifi-signal-mapper",
        "name": "WiFi Signal Mapper & Spectrum Histogram",
        "category": "Beginner",
        "dir": "projects/beginner/11-wifi-signal-mapper",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "pong",
        "name": "Retro 60 FPS OLED Arcade Pong",
        "category": "Games",
        "dir": "projects/games/01-pong",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "snake",
        "name": "Nokia-Style OLED Snake with NVS High Score",
        "category": "Games",
        "dir": "projects/games/02-snake",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "wifi-deauth-detector",
        "name": "802.11 Deauthentication Packet Detector",
        "category": "Security",
        "dir": "projects/security/01-wifi-deauth-detector",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "wardriver",
        "name": "Continuous 2.4 GHz WiGLE Wardriver",
        "category": "Security",
        "dir": "projects/security/03-wardriver",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "led-strip-controller",
        "name": "Addressable NeoPixel / WS2812B Controller",
        "category": "Smart Home",
        "dir": "projects/smart-home/09-led-strip-controller",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "garage-door",
        "name": "Smart Garage Door Opener & Monitor",
        "category": "Smart Home",
        "dir": "projects/smart-home/03-garage-door",
        "envs": [
            {"env": "esp32dev", "chip": "ESP32", "boot_offset": 4096}
        ]
    },
    {
        "slug": "security-camera",
        "name": "ESP32-CAM MJPEG Video Streamer",
        "category": "Camera",
        "dir": "projects/esp32-cam/01-security-camera",
        "envs": [
            {"env": "esp32cam", "chip": "ESP32", "boot_offset": 4096}
        ]
    }
]

def compile_and_export():
    FIRMWARE_DEST_DIR.mkdir(parents=True, exist_ok=True)
    manifest_catalog = []

    for proj in PROJECTS:
        slug = proj["slug"]
        name = proj["name"]
        proj_dir = REPO_ROOT / proj["dir"]
        print(f"\n==================================================")
        print(f"📦 Processing: {name} ({slug})")
        print(f"==================================================")

        builds = []
        for env_info in proj["envs"]:
            env_name = env_info["env"]
            chip = env_info["chip"]
            boot_offset = env_info["boot_offset"]

            target_out = FIRMWARE_DEST_DIR / slug / env_name
            target_out.mkdir(parents=True, exist_ok=True)

            print(f"🔨 Compiling environment '{env_name}' in {proj_dir}...")
            cmd = [PIO_BIN, "run", "-d", str(proj_dir), "-e", env_name]
            res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
            if res.returncode != 0:
                print(f"❌ Build failed for {slug} [{env_name}]:")
                print(res.stdout[-1500:])
                sys.exit(1)

            # Locate compiled binary outputs
            pio_build = proj_dir / ".pio" / "build" / env_name
            bootloader_src = pio_build / "bootloader.bin"
            partitions_src = pio_build / "partitions.bin"
            firmware_src = pio_build / "firmware.bin"

            # Destination paths relative to web-flasher directory
            boot_dst = target_out / "bootloader.bin"
            part_dst = target_out / "partitions.bin"
            firm_dst = target_out / "firmware.bin"

            shutil.copy2(bootloader_src, boot_dst)
            shutil.copy2(partitions_src, part_dst)
            shutil.copy2(firmware_src, firm_dst)

            rel_boot = f"firmware/{slug}/{env_name}/bootloader.bin"
            rel_part = f"firmware/{slug}/{env_name}/partitions.bin"
            rel_app0 = "firmware/boot_app0.bin"
            rel_firm = f"firmware/{slug}/{env_name}/firmware.bin"

            builds.append({
                "chipFamily": chip,
                "parts": [
                    {"path": rel_boot, "offset": boot_offset},
                    {"path": rel_part, "offset": 32768},
                    {"path": rel_app0, "offset": 57344},
                    {"path": rel_firm, "offset": 65536}
                ]
            })

            # Clean .pio to keep workspace tidy
            shutil.rmtree(proj_dir / ".pio", ignore_errors=True)

        manifest_data = {
            "name": name,
            "version": "1.0.0",
            "home_assistant_domain": "esphome",
            "new_install_prompt_erase": False,
            "builds": builds
        }

        manifest_path = WEB_FLASHER_DIR / f"manifest-{slug}.json"
        with open(manifest_path, "w", encoding="utf-8") as f:
            json.dump(manifest_data, f, indent=2)

        print(f"✅ Generated manifest: {manifest_path.name}")
        manifest_catalog.append({
            "slug": slug,
            "name": name,
            "category": proj.get("category", "General"),
            "manifest": f"manifest-{slug}.json"
        })

    print(f"\n🎉 Successfully compiled and packaged {len(PROJECTS)} projects for Web Flasher!")

if __name__ == "__main__":
    compile_and_export()
