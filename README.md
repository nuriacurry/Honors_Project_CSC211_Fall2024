# BMCC HomeQuest

A specialized housing search application designed to help BMCC students find affordable housing near campus. Built with Qt and C++.

## Features

- **Student-Specific Authentication**
  - Secure login system with BMCC email verification
  - Student profile management

- **Advanced Search Functionality**
  - Filter by price range ($500-$3000)
  - Search by borough and neighborhood
  - Filter by amenities (furnished, utilities included, etc.)
  - Distance to BMCC calculations
  - Transit route information

- **User-Friendly Interface**
  - Modern, intuitive design
  - Responsive layout
  - Easy-to-use filtering system
  - Detailed property views

- **Helpful Tools**
  - Pre-formatted message templates for landlord communication
  - Housing search tips for students
  - Document requirement checklists
  - One-click map viewing
  - Favorites system

## Technology Stack

- C++ 17
- Qt 6.5.3
- Qt Creator IDE
- File-based storage system using CSV
- Object-oriented architecture

## Installation

### Prerequisites
- Qt 6.5.3 or higher
- C++ compiler (MinGW for Windows, GCC for Linux/Mac)
- Qt Creator IDE

### Setup Steps
1. Clone the repository:
```bash
git clone https://github.com/yourusername/bmcc-homequest.git
```

2. Open Qt Creator and select "Open Project"

3. Navigate to the cloned repository and select the `.pro` file

4. Configure the project for your system

5. Build and run the application

## Project Structure

```
bmcc-homequest/
├── src/
│   ├── main.cpp
│   ├── loginwindow.cpp
│   ├── mainwindow.cpp
│   ├── database.cpp
│   └── ...
├── include/
│   ├── loginwindow.h
│   ├── mainwindow.h
│   ├── database.h
│   └── ...
├── resources/
│   ├── images/
│   └── data/
└── docs/
    ├── uml/
    └── flowcharts/
```

## Classes

- `LoginWindow`: Handles user authentication
- `MainWindow`: Main application interface
- `Database`: Manages data storage and retrieval
- `ListingDetail`: Property information display
- `User`: User profile management
- `PropertyManager`: Listing management

## Features in Development

- Real-time data integration with listing services
- Advanced mapping features
- Additional student-specific filters
- In-app landlord-student communication system
- Landlord portal for listing management

## Challenges and Solutions

- **Data Source**: Created a curated CSV database of verified listings
- **Image Management**: Implemented local storage system for listing images
- **Communication**: Developed template system for landlord contact
- **Maps Integration**: Created browser redirect solution for mapping

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

## Contact

- Nuria Siddiqa - nuri20024@gmail.com

## Acknowledgments

- Dr. Azhar - Project Supervisor
- BMCC Computer Science Department
- Qt Documentation and Community
