#!/usr/bin/env python3
"""
ESP32 Community: Unified Developer & Project CLI Utility
Usage:
    python3 tools/esp32_cli.py list
    python3 tools/esp32_cli.py build <project-path-or-name>
    python3 tools/esp32_cli.py test
    python3 tools/esp32_cli.py serve [--port 8000]
"""

import os
import sys
import argparse
import shutil
import subprocess
import unittest
from pathlib import Path
from http.server import HTTPServer, SimpleHTTPRequestHandler

REPO_ROOT = Path(__file__).resolve().parent.parent

# Detect PlatformIO binary
PIO_BIN = shutil.which("pio") or str(Path.home() / "Library/Python/3.9/bin/pio")


def cmd_list(args):
    """Scan and list all projects with their target environments."""
    print("\n" + "=" * 80)
    print("⚡ ESP32 COMMUNITY PROJECT REGISTRY")
    print("=" * 80)

    projects = []
    for pio_path in sorted(REPO_ROOT.glob("projects/**/platformio.ini")):
        proj_dir = pio_path.parent
        category = proj_dir.parent.name
        slug = proj_dir.name
        rel_path = proj_dir.relative_to(REPO_ROOT)
        
        # Read default envs or boards
        envs = []
        with open(pio_path, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line.startswith("[env:"):
                    env_name = line.split("[env:")[1].rstrip("]")
                    envs.append(env_name)

        projects.append({
            "category": category.upper(),
            "name": slug,
            "path": str(rel_path),
            "envs": ", ".join(envs) if envs else "default"
        })

    # Print table
    print(f"{'CATEGORY':<14} | {'PROJECT SLUG':<30} | {'TARGET ENVS':<26}")
    print("-" * 14 + "-+-" + "-" * 30 + "-+-" + "-" * 26)
    for p in projects:
        print(f"{p['category']:<14} | {p['name']:<30} | {p['envs']:<26}")

    print("-" * 80)
    print(f"Total Projects Registered: {len(projects)}")
    print("=" * 80 + "\n")


def cmd_build(args):
    """Build a project using PlatformIO."""
    target = args.target.strip()
    target_path = None

    # Check if target is a path
    direct_path = REPO_ROOT / target
    if direct_path.is_dir() and (direct_path / "platformio.ini").exists():
        target_path = direct_path
    else:
        # Search by folder name
        for match in REPO_ROOT.glob(f"projects/**/{target}"):
            if match.is_dir() and (match / "platformio.ini").exists():
                target_path = match
                break

    if not target_path:
        print(f"❌ Error: Project '{target}' not found in projects/!")
        sys.exit(1)

    print(f"\n🔨 Building project in: {target_path.relative_to(REPO_ROOT)}...")
    res = subprocess.run([PIO_BIN, "run"], cwd=target_path)
    if res.returncode == 0:
        print("✅ Build PASSED successfully!\n")
    else:
        print("❌ Build FAILED!\n")
        sys.exit(res.returncode)


def cmd_test(args):
    """Run Web Flasher compatibility test suite."""
    print("\n🧪 Running Web Flasher & PlatformIO Test Suite...\n")
    suite = unittest.defaultTestLoader.discover(str(REPO_ROOT / "tests"))
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    if not result.wasSuccessful():
        sys.exit(1)


def cmd_serve(args):
    """Serve web-flasher locally for testing."""
    web_dir = REPO_ROOT / "web-flasher"
    port = args.port

    class CustomHandler(SimpleHTTPRequestHandler):
        def __init__(self, *a, **k):
            super().__init__(*a, directory=str(web_dir), **k)

        def end_headers(self):
            # Headers for Web Serial API compatibility
            self.send_header("Cross-Origin-Opener-Policy", "same-origin")
            self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            super().end_headers()

    server = HTTPServer(("0.0.0.0", port), CustomHandler)
    print(f"\n🌐 Local Web Flasher Server running at http://localhost:{port}/")
    print("Press Ctrl+C to stop.\n")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nServer stopped.")


def main():
    parser = argparse.ArgumentParser(description="ESP32 Community Developer CLI")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # Subcommand: list
    sub_list = subparsers.add_parser("list", help="List all registered projects")
    sub_list.set_defaults(func=cmd_list)

    # Subcommand: build
    sub_build = subparsers.add_parser("build", help="Compile a project with PlatformIO")
    sub_build.add_argument("target", help="Project path or directory name")
    sub_build.set_defaults(func=cmd_build)

    # Subcommand: test
    sub_test = subparsers.add_parser("test", help="Run automated test suite")
    sub_test.set_defaults(func=cmd_test)

    # Subcommand: serve
    sub_serve = subparsers.add_parser("serve", help="Launch local Web Flasher HTTP server")
    sub_serve.add_argument("--port", type=int, default=8000, help="Port to listen on (default: 8000)")
    sub_serve.set_defaults(func=cmd_serve)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
