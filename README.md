# saleae-logic-SimpleParallelNoClock-Analyzer

Plugin for Saleae Logic Software. Analyzer for parallel data communication without a clock signal. The data word transmitted is re-evaluated on every data transition.

## Features & Modifications (webs1821 fork)

This fork builds upon the original plugin and incorporates essential fixes from `maxmitti`, along with new capabilities specifically designed for debugging 16-bit systems (like the Intel 8086).

### 1. Enhancements by `maxmitti` (Base Fork)
* **FrameV2 Support**: Full compatibility with the new Logic 2 Data Table and High-Level Analyzers (HLAs). Data is saved as a `Byte` for 8 channels or `Integer` for up to 16 channels.
* **Logic 2 Freeze Fix**: Resolves an issue where the analyzer would crash or hang Logic 2 during a Live Capture if no new edges were detected.
* **Basic Glitch Skipping**: Ignores glitches that are only 1 sample long to prevent false data reads.
* **Dynamic Number Formatting**: Accurately formats display strings based on the actual number of active channels instead of hardcoded 8 or 16 bits.

### 2. Enhancements in this Version (webs1821)
* **Configurable Glitch Filter (Settling Time)**: Added a new `Glitch Filter (ns)` setting in the UI. For clockless systems like the 8086, data lines settle at slightly different times, causing transient states. You can now specify a minimum settling time (in nanoseconds) that the bus must be stable before the frame is evaluated.
* **Endianness Byte Swap (16-bit)**: Added a `Swap Bytes (16-bit / Endianness)` toggle. Since x86 is Little Endian, memory reads of a 16-bit word will place the LSB at the lower address and MSB at the higher address. This option mathematically swaps the lower and upper 8 bits so the decoded values match standard Left-to-Right hex dumps (e.g., swapping `0x00EA` to `0xEA00`) without needing to rewire probes in the UI.

---

Documentation for the Saleae Logic Analyzer SDK can be found here:
https://github.com/saleae/SampleAnalyzer

That documentation includes:

- Detailed build instructions
- Debugging instructions
- Documentation for CI builds
- Details on creating your own custom analyzer plugin

# Installation Instructions

To use this analyzer, simply download the latest release zip file from this github repository, unzip it, then install using the instructions found here:

https://support.saleae.com/faq/technical-faq/setting-up-developer-directory

# Publishing Releases

This repository is setup with Github Actions to automatically build PRs and commits to the master branch.

However, these artifacts automatically expire after a time limit.

To create and publish cross-platform releases, simply push a commit tag. This will automatically trigger the creation of a release, which will include the Windows, Linux, and MacOS builds of the analyzer.

# A note on downloading the MacOS Analyzer builds

This section only applies to downloaded pre-built protocol analyzer binaries on MacOS. If you build the protocol analyzer locally, or acquire it in a different way, this section does not apply.

Any time you download a binary from the internet on a Mac, wether it be an application or a shared library, MacOS will flag that binary for "quarantine". MacOS then requires any quarantined binary to be signed and notarized through the MacOS developer program before it will allow that binary to be executed.

Because of this, when you download a pre-compiled protocol analyzer plugin from the internet and try to load it in the Saleae software, you will most likely see an error message like this:

> "libSimpleSerialAnalyzer.so" cannot be opened because th developer cannot be verified.

Signing and notarizing of open source software can be rare, because it requires an active paid subscription to the MacOS developer program, and the signing and notarization process frequently changes and becomes more restrictive, requiring frequent updates to the build process.

The quickest solution to this is to simply remove the quarantine flag added by MacOS using a simple command line tool.

Note - the purpose of code signing and notarization is to help end users be sure that the binary they downloaded did indeed come from the original publisher and hasn't been modified. Saleae does not create, control, or review 3rd party analyzer plugins available on the internet, and thus you must trust the original author and the website where you are downloading the plugin. (This applies to all software you've ever downloaded, essentially.)

To remove the quarantine flag on MacOS, you can simply open the terminal and navigate to the directory containing the downloaded shared library.

This will show what flags are present on the binary:

```sh
xattr libSimpleSerialAnalyzer.so
# example output:
# com.apple.macl
# com.apple.quarantine
```

This command will remove the quarantine flag:

```sh
xattr -r -d com.apple.quarantine libSimpleSerialAnalyzer.so
```

To verify the flag was removed, run the first command again and verify the quarantine flag is no longer present.

## Building your Analyzer

CMake and a C++ compiler are required. Instructions for installing dependencies can be found here:
https://github.com/saleae/SampleAnalyzer

The fastest way to use this analyzer is to download a release from github. Local building should only be needed for making your own changes to the analyzer source.

### Windows

```bat
mkdir build
cd build
cmake .. -A x64
cmake --build .
:: built analyzer will be located at SampleAnalyzer\build\Analyzers\Debug\SimpleSerialAnalyzer.dll
```

### MacOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```

### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```
