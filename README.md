# 🌬️ 2D or 3D Ventilator Fan Simulation with OpenGL/GLUT

![OpenGL/GLUT Ventilator Fan](https://via.placeholder.com/800x600/4a4a4a/ffffff?text=Ventilator+Fan+Simulation)
*Visualize and interact with a realistic ventilator fan in 2D or 3D using OpenGL/GLUT*

---

## 🚀 **Overview**

Create an **interactive 2D or 3D ventilator fan simulation** using **OpenGL/GLUT** in C++. This project lets you:
✅ **Visualize** a fan in two modes (2D or 3D)
✅ **Control** fan speed, power, and rotation
✅ **Simulate** airflow with dynamic particles
✅ **Customize** colors, dimensions, and physics

Perfect for **game developers, graphics enthusiasts, and students** learning OpenGL!

---

## ✨ **Features**

🔹 **Dual Rendering Modes** – Choose between **2D or 3D** visualization
🔹 **Interactive Controls** – Toggle power, adjust speed, and rotate the fan
🔹 **Physics Simulation** – Smooth acceleration/deceleration with realistic motion
🔹 **Customizable Colors** – Change fan, desk, and blade colors
🔹 **3D Camera Controls** – Rotate, zoom, and pan in 3D mode
🔹 **OpenGL/GLUT-Based** – Uses standard OpenGL primitives for rendering

---

## 🛠️ **Tech Stack**

| **Category**       | **Technology**                     |
|--------------------|-----------------------------------|
| **Language**       | C++                               |
| **Graphics**       | OpenGL (GLUT for window management) |
| **Physics**        | Custom acceleration/deceleration  |
| **Build Tool**     | Makefile (for compilation)        |
| **OS Support**     | Linux, macOS, Windows (with GLUT)  |

### **System Requirements**
- **C++11 or later** compiler (GCC, Clang, MSVC)
- **OpenGL/GLUT** support (usually included in standard libraries)
- **Basic OpenGL knowledge** (helpful but not required)

---

## 📦 **Installation**

### **Prerequisites**
1. **Install OpenGL/GLUT** (usually comes with standard libraries):
   - **Linux (Debian/Ubuntu):**
     ```bash
     sudo apt-get install freeglut3-dev libglu1-mesa-dev
     ```
   - **macOS (Homebrew):**
     ```bash
     brew install freeglut
     ```
   - **Windows:** Download [GLUT for Windows](http://freeglut.sourceforge.net/) and ensure OpenGL drivers are installed.

2. **C++ Compiler** (GCC, Clang, or MSVC)

---

### **Quick Start (Linux/macOS/Windows)**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/sniperras/2D-or-3D-ventilator-fan-with-opengl-glut.git
   cd 2D-or-3D-ventilator-fan-with-opengl-glut
   ```

2. **Compile & Run (2D Mode):**
   ```bash
   g++ -o ventilator_2d 2D/main.cpp -lGL -lGLU -lglut
   ./ventilator_2d
   ```

3. **Compile & Run (3D Mode):**
   ```bash
   g++ -o ventilator_3d 3D/main.cpp -lGL -lGLU -lglut
   ./ventilator_3d
   ```

4. **Controls:**
   - **2D Mode:** Use mouse to rotate, `P` to toggle power, `1-5` to adjust speed.
   - **3D Mode:** Left-click & drag to rotate, right-click & drag to zoom, `P` to toggle power, `1-5` to adjust speed.

---

### **Alternative: Docker (Optional)**
If you prefer Docker, check out the `Dockerfile` (if provided) or build your own:
```dockerfile
FROM ubuntu:latest
RUN apt-get update && apt-get install -y freeglut3-dev libglu1-mesa-dev
COPY . /app
WORKDIR /app
RUN g++ -o ventilator_2d 2D/main.cpp -lGL -lGLU -lglut
CMD ["./ventilator_2d"]
```

---

## 🎯 **Usage**

### **Basic 2D Fan Simulation**
```cpp
// Inside `main.cpp`, the fan is controlled via global variables:
bool fanOn = false;          // Toggle with 'P' key
int fanSpeedLevel = 0;       // Adjust with '1'-'5' keys
float rotationSpeed = 0.0f;  // Automatically updated by physics
```

### **Advanced 3D Controls**
- **Camera Movement:**
  - Left-click & drag → Rotate view
  - Right-click & drag → Zoom in/out
- **Fan Interaction:**
  - `P` → Toggle power
  - `1-5` → Set speed level
  - `R` → Reset fan position

### **Customizing Colors**
Modify the `fanColor`, `bladeColors`, and `deskColor` arrays in `main.cpp`:
```cpp
float fanColor[3] = {0.4f, 0.4f, 0.4f};  // Default gray
// Change to red:
float fanColor[3] = {1.0f, 0.0f, 0.0f};
```

---

## 📁 **Project Structure**
```
2D-or-3D-ventilator-fan-with-opengl-glut/
│
├── 2D/                  # 2D Fan Simulation
│   └── main.cpp         # Core 2D rendering logic
│
├── 3D/                  # 3D Fan Simulation
│   └── main.cpp         # Core 3D rendering logic
│
├── README.md            # This file (your guide!)
├── LICENSE              # Project license
└── Dockerfile           # (Optional) Docker setup
```

---

## 🔧 **Configuration**

### **Environment Variables**
None required—just compile and run!

### **Customization Options**
1. **Fan Dimensions:**
   Modify `drawFan()` and `drawFanBlades()` functions to adjust size.
2. **Physics:**
   Change `acceleration` and `deceleration` constants in `main.cpp`.
3. **Window Size:**
   Update `windowWidth` and `windowHeight` globals.

---

## 🤝 **Contributing**

We welcome contributions! Here’s how you can help:

### **Steps to Contribute**
1. **Fork the repository** → [GitHub Fork Guide](https://docs.github.com/en/get-started/quickstart/fork-a-repo)
2. **Clone your fork:**
   ```bash
   git clone https://github.com/sniperras/2D-or-3D-ventilator-fan-with-opengl-glut.git
   ```
3. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature
   ```
4. **Make changes** → Test thoroughly!
5. **Commit & push:**
   ```bash
   git commit -m "Add new feature: X"
   git push origin feature/your-feature
   ```
6. **Open a Pull Request** → [GitHub PR Guide](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request)

### **Code Style Guidelines**
- Use **consistent indentation** (4 spaces).
- Follow **C++11/14/17 standards**.
- Add **comments** for complex logic.
- Keep **functions modular** (e.g., `drawFan()`, `updatePhysics()`).

### **Pull Request Process**
1. Ensure tests pass (if any).
2. Update documentation (this `README.md`).
3. Link related issues (if applicable).

---

## 📝 **License**

This project is licensed under the **MIT License** – see [`LICENSE`](LICENSE) for details.

---

## 👥 **Authors & Contributors**

👤 **Maintainer:** [Your Name](https://github.com/sniperras)
🤝 **Contributors:** [List of contributors](https://github.com/sniperras/2D-or-3D-ventilator-fan-with-opengl-glut/graphs/contributors)

---

## 🐛 **Issues & Support**

### **Reporting Bugs**
Found an issue? Open a **GitHub Issue** with:
- Steps to reproduce.
- Expected vs. actual behavior.
- Screenshots (if applicable).

### **Getting Help**
- **Discussions:** [GitHub Discussions](https://github.com/sniperras/2D-or-3D-ventilator-fan-with-opengl-glut/discussions)
- **Community:** Join our [OpenGL C++ Slack](https://opengl-cpp.slack.com) (invite link in README).

### **FAQ**
❓ **Why does the fan not rotate smoothly?**
→ Ensure `glutTimerFunc()` is used for animation (already implemented).

❓ **How do I add more blades?**
→ Modify `bladeColors` array and update `drawFanBlades()` logic.

---

## 🗺️ **Roadmap**

| **Feature**               | **Status**       | **Description**                          |
|---------------------------|------------------|------------------------------------------|
| **4K Resolution Support** | ⚠️ Planned       | Optimize for high-DPI displays.          |
| **Particle System Upgrade** | 🚧 In Progress | Add GPU-accelerated airflow particles.   |
| **Touchscreen Support**   | 💡 Idea          | Adapt for mobile devices.                |
| **VR Integration**        | 🌟 Future        | Use OpenVR for VR fan simulation.        |

---

## 🎉 **Get Started Today!**

👉 **Star this repo** to support the project!
👉 **Fork & modify** to create your own fan design!
👉 **Contribute** to make it even better!

---
**Happy coding!** 🚀
```
