# 🔒 Security Policy

## Supported Versions

The following versions of the ESP32 Community repository are currently supported with security updates:

| Version | Supported |
|---------|-----------|
| 1.x.x (latest) | ✅ Yes |
| < 1.0.0 | ❌ No |

We recommend always using the latest version from the `main` branch.

---

## Reporting a Vulnerability

We take the security of the ESP32 Community seriously. If you discover a security vulnerability in any project, firmware, or infrastructure in this repository, we appreciate your help in disclosing it responsibly.

### How to Report

**⚠️ Do NOT open a public GitHub issue for security vulnerabilities.**

Instead, please report vulnerabilities via email:

📧 **[tushar.inseec@gmail.com](mailto:tushar.inseec@gmail.com)**

### What to Include

Please include as much of the following information as possible:

1. **Description** of the vulnerability
2. **Affected component** (which project, file, or firmware)
3. **Steps to reproduce** the vulnerability
4. **Potential impact** (what could an attacker do?)
5. **Suggested fix** (if you have one)
6. **Your contact information** (for follow-up)

### What to Expect

| Step | Timeline |
|------|----------|
| Acknowledgment | Within **48 hours** of your report |
| Initial assessment | Within **1 week** |
| Fix development | Within **2 weeks** (for critical issues) |
| Public disclosure | After fix is released (coordinated with reporter) |

---

## Scope

### In Scope

The following are considered valid security concerns:

- **Firmware vulnerabilities** — Buffer overflows, remote code execution, authentication bypasses in ESP32 project firmware
- **Network security** — Unencrypted communications, weak authentication, exposed API keys or credentials in committed code
- **Web Flasher security** — XSS, CSRF, or other web vulnerabilities in the Web Flasher application
- **Supply chain** — Compromised dependencies, malicious libraries referenced in projects
- **Credential exposure** — Accidentally committed WiFi passwords, API keys, tokens, or secrets

### Out of Scope

The following are NOT considered security vulnerabilities in this repository:

- **Known ESP32 hardware limitations** — Issues inherent to the ESP32 chip itself (report to Espressif)
- **Educational security projects** — Projects in the `security/` category that intentionally demonstrate vulnerabilities for learning purposes (these are clearly labeled)
- **User misconfiguration** — Issues arising from users not following setup instructions
- **Denial of service on personal devices** — The projects run on personal hardware, not public servers

---

## Security Best Practices for Contributors

When contributing projects to the ESP32 Community:

### Code Security

- **NEVER** commit WiFi passwords, API keys, tokens, or secrets
- Use `secrets.h` or `config.h` with entries in `.gitignore`
- Always provide a `secrets.h.example` or `config.py.example` template
- Validate all user inputs, especially from network sources
- Use HTTPS for all API communications
- Implement proper authentication for web interfaces

### Firmware Security

- Enable **Secure Boot** documentation where applicable
- Use **flash encryption** for sensitive firmware
- Implement **OTA update verification** (signed updates)
- Document any network-facing services and their security implications
- Avoid running services on well-known ports without authentication

### Network Security

- Use TLS/SSL for MQTT connections (`mqtts://`)
- Implement proper WiFi security (WPA2/WPA3)
- Never expose debug interfaces (JTAG, serial) in production firmware
- Use proper certificate pinning for sensitive communications
- Document firewall rules for network-dependent projects

### Dependencies

- Pin library versions in `platformio.ini` and `lib_deps`
- Use official library sources (Arduino Library Manager, PlatformIO Registry)
- Review third-party libraries before including them
- Keep dependencies updated to patch known vulnerabilities

---

## Responsible Disclosure

We follow a **responsible disclosure** process:

1. **Reporter** sends vulnerability details to [tushar.inseec@gmail.com](mailto:tushar.inseec@gmail.com)
2. **We acknowledge** receipt within 48 hours
3. **We investigate** and develop a fix
4. **We coordinate** a disclosure timeline with the reporter
5. **We release** the fix and publish a security advisory
6. **We credit** the reporter (unless they prefer anonymity)

We will not take legal action against researchers who:
- Make a good faith effort to follow this disclosure policy
- Do not access or modify other users' data
- Do not exploit the vulnerability beyond what is necessary for verification
- Report vulnerabilities promptly

---

## Security Advisories

Security advisories will be published via:

- **GitHub Security Advisories** — [github.com/tushar2704/ESP32-Community/security/advisories](https://github.com/tushar2704/ESP32-Community/security/advisories)
- **CHANGELOG.md** — Noted in the changelog under `### Security`
- **GitHub Discussions** — Announcement in the community

---

## Contact

- 📧 **Email**: [tushar.inseec@gmail.com](mailto:tushar.inseec@gmail.com)
- 🔗 **GitHub**: [@tushar2704](https://github.com/tushar2704)

---

<p align="center">
  <sub>Thank you for helping keep the ESP32 Community safe and secure! 🔐</sub>
</p>
