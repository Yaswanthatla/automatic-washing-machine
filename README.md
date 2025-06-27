# 🚿 Automatic Washing Machine – Embedded Systems Project

This is an embedded C project that simulates the working of an automatic washing machine using the PIC16F877A microcontroller. Developed during my internship at Emertex Embedded Solutions.

---

## 📌 Features
- 12 preset washing programs (e.g. Daily, Delicates, EcoCotton, etc.)
- 5 water level options (Auto to Max)
- CLCD-based user interface
- Digital keypad input (Start, Stop, Next)
- Timer-based washing logic using Timer2 interrupts
- Safety check: Door open detection using RB0 pin
- Real-time countdown and buzzer alert on completion

---

## 🛠 Technologies Used
- **Microcontroller:** PIC16F877A
- **Language:** Embedded C (XC8 Compiler)
- **IDE:** MPLAB X IDE
- **Simulation:** PICSimLab / Proteus

---

## 🎮 Controls
| Button | Function |
|--------|----------|
| SW4    | Navigate programs and water level |
| SW5    | Start the selected program |
| SW6    | Pause/Stop the machine |
| RB0 Pin | Door open/close simulation |

---

## 💡 Description
When powered on, the user can choose a washing program and water level using the keypad. Based on the selection,
the machine calculates appropriate wash time. The logic divides the cycle into washing, rinsing, and spinning phases.
It uses a CLCD to show current operation and time left. If the door is opened mid-cycle, the system pauses and alerts the user.

---

## 👨‍💻 Developed By
**Yaswanth Atla**  
B.Tech – ECE | Internship at Emertex Embedded Solutions  
[GitHub](https://github.com/Yaswanthatla/automatic-washing-machine)
