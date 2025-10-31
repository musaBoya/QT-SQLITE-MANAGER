QT-SQLite Manager

A Template Project: This repository serves as a reference implementation and learning resource for developing SQLite database management applications with Qt Framework (QML and C++). It demonstrates best practices for creating intuitive visual interfaces to manage database records with image support.

✨ Features

  🎯 Complete CRUD Operations: Create, Read, Update, and Delete database records through an intuitive graphical interface
  
  🖼️ Image Support: Attach and display images with each database record - automatically copies images to project directory with timestamped filenames
  
  🔍 Data Visualization: Clean table view for browsing and managing all database records
  
  ⚙️ Settings Management: Persistent application preferences using QSettings for window size, background images, and language settings
  
  🌍 Localization Ready: Integrated Qt Linguist support for multiple language interfaces
  
  🔄 Auto-Refresh: Main data table automatically updates after record additions or modifications
  

🚀 Quick Start
Prerequisites

    -Qt Framework (with QML and C++ support)
  
    -C++ compiler (GCC, MSVC, or compatible)
  
    -CMake or qmake build system
  
    -SQLite database engine

Installation & Setup

  1. Clone the repository
  bash

    git clone https://github.com/musaBoya/QT-SQLITE-MANAGER.git
    cd QT-SQLITE-MANAGER

2.Create and enter build directory
bash

    mkdir build && cd build

3.Configure project with CMake
bash

    cmake ..

4.Build the project
bash

    make

5.For parallel builds, use: make -j$(nproc)

Run the application
bash

    ./sqldesktop

💾 Database Schema

Your application requires this table structure. Create it using this SQL command:
sql

    CREATE TABLE TABLE (
      id INTEGER PRIMARY KEY,
      TABLE_TITLE TEXT NOT NULL,
      TABLE_DATE DATE,
      TABLE_TAG TEXT,
      TABLE_VOTE TEXT,
      TABLE_IMAGE TEXT
    );

👨‍💻 Developer

    Musa Boya - https://github.com/musaBoya
