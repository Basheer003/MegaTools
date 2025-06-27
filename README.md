# MegaTools Developer Tools

MegaTools is a handy, all-in-one desktop application built with Qt/C++ that provides a collection of essential tools for developers. It's designed to streamline common development tasks by bringing multiple utilities into a single, easy-to-use interface.

## Features

The application is organized into a clean, tabbed interface for easy access to its various tools:

### 1. Text / Base64 / URL Converter

A versatile tool for encoding and decoding text data.

-   **Text to Hex** (and vice-versa)
    
-   **Text to Base64** (and vice-versa)
    
-   **URL Encoding** (Percent Encoding)
    
-   **URL Decoding**
    

### 2. Hashing Calculator

Instantly generate cryptographic hashes from any text input. The hashes update in real-time as you type.

-   **MD5**
    
-   **SHA-1**
    
-   **SHA-256**
    
-   **SHA-512**
    

### 3. Number Base Converter

Seamlessly convert numbers between different numeral systems. Entering a value in any base will automatically update all other fields.

-   **Decimal**
    
-   **Hexadecimal**
    
-   **Binary**
    
-   **Octal**
    

### 4. Unix Timestamp Converter

Easily switch between Unix time (seconds since the epoch) and a human-readable date and time format.

-   **Timestamp to UTC Date/Time**
    
-   **UTC Date/Time to Timestamp**
    
-   **"Now" button** to get the current timestamp.
    

### 5. JSON Tools

Format and validate JSON documents.

-   Paste any JSON text and click **Format & Validate**.
    
-   The tool will "prettify" valid JSON with proper indentation.
    
-   It will report any syntax errors and their location if the JSON is invalid.
    

### 6. Color Converter

Convert between popular color formats and use a visual color picker.

-   **HEX, RGB, and HSL** value conversions.
    
-   All fields are synchronized: changing one updates the others.
    
-   A **color swatch** provides a visual preview.
    
-   Use the **"Pick Color"** button to select a color visually.
    

### 7. Regex Tester

Test and debug regular expressions in real-time.

-   Enter your regex pattern and a test string.
    
-   A list of all matches is displayed instantly.
    
-   The status bar provides feedback on pattern validity and the number of matches found.
    

### 8. UUID Generator

Quickly generate and copy universally unique identifiers.

-   Generate a Version 4 (random) UUID with a single click.
    
-   **Copy** the generated UUID to the clipboard.
    

### 9. Lorem Ipsum Generator

Create placeholder text for your mockups and designs.

-   Generate a specific number of **words, sentences, or paragraphs**.
    
-   Ideal for populating UI elements during development.
    

## Getting Started

### Prerequisites

-   Qt 5 or newer (Qt 6+ recommended)
    
-   A C++ compiler (GCC, Clang, MSVC, etc.)
    

### Building the Project

1.  Open the `MegaTools.pro` file in **Qt Creator**.
    
2.  Configure the project for your desired build kit (e.g., Desktop Qt MinGW, MSVC).
    
3.  Click the **Build** button (or press `Ctrl+B`).
    
4.  Click the **Run** button (or press `Ctrl+R`) to launch the application.
    

## How to Use

1.  Launch the MegaTools application.
    
2.  Select the tab corresponding to the tool you wish to use.
    
3.  Enter your data in the input fields.
    
4.  The output will be generated automatically or after clicking a conversion button.
    

## Contributing

Contributions are welcome! If you have ideas for new tools or improvements, feel free to fork the repository, make your changes, and submit a pull request.
