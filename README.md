# 💬 Freescord — Real-Time Messaging System

<p align="center">
  <strong>Multi-client chat application using TCP sockets & multithreading in C</strong>
</p>

---

## 📋 Overview

**Freescord** is a client-server messaging system inspired by Discord, built in pure C.

It demonstrates low-level networking, concurrency, and system programming concepts.

---

## 🏗️ Architecture
    ┌───────────────┐
    │   Client 1    │
    └──────┬────────┘
           │
    ┌──────▼────────┐
    │               │
    │    Server     │
    │ (Multi-thread)│
    │               │
    └──────┬────────┘
           │
    ┌──────▼────────┐
    │   Client 2    │
    └───────────────┘


- Server handles multiple clients concurrently  
- Each client communicates via TCP sockets  
- Threads ensure real-time responsiveness  

---

## 🚀 Features

- 🔌 TCP socket communication  
- 👥 Multi-client support  
- ⚡ Multi-threaded server  
- 💬 Real-time messaging  
- 🧠 Efficient memory management  

---

## ⚙️ How it works

1. Server starts and listens on a port  
2. Clients connect to the server  
3. Each client is handled in a separate thread  
4. Messages are broadcast to all connected users  

---

## 🚀 Quick Start

```bash
# Compile server
gcc server.c -o server -lpthread

# Compile client
gcc client.c -o client

# Run server
./server

# Run client (in another terminal)
./client
