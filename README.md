# DCU - DIM command-line utils

## Building

### Requirements
- **C++20**
- **CMake** (version > 3.1)

### Build Instructions

```bash
mkdir build
cmake -S . -B build
cmake --build build
```

## Usage

### Overview
`DCU` is a terminal tool designed to simplify the development of DIM-related software. In its current version, `DCU` allows you to:

- Save data from multiple services to files.
- Send single-line and multi-line commands from the terminal—or define them in a configuration file for repeated use.
- Correlate command services with response services.

### Configuration File

The configuration file should be populated with information about the services of interest. Currently, `DCU` supports two types of services:

- **Command services** (`DimCommand`)
- **String services** (`DimInfo`)

#### File Format

The configuration file is an XML file with the root node `Configuration`. Each service is defined using a `Service` node with the following attributes:

- **type**: Use `command` for command services and `service_info` for string services.
- **name**: The service name.
- **alias**: An alias used to reference the service in other sections of the configuration file.

#### Defining a Command Service

A command service node may contain two child nodes:

1. **commands**: Contains a list of predefined commands (each specified with a `command` node). Each command must have a name (provided via the `name` attribute). The command message can be provided as the node’s value or read from a file (using the `file` attribute).
2. **response_on**: If you expect a response from another service (or from one within a list of services), list the service names or aliases within `service` nodes. (Ensure these services are defined elsewhere in the configuration file.)

```xml
<Service type="command" name="MOCK-DIM/TEST/IN" alias="TEST/IN">
    <commands>
        <command name="cmd-a">cmd-a</command>
        <command name="cmd-b" file="/path/to/file"/>
    </commands>
    <response_on>
        <service>TEST/A</service>
    </response_on>
</Service>
```

#### Defining a String Service

In addition to basic service information, you can specify the path to an output file.

```xml
<Service type="service_info" name="MOCK-DIM/TEST/A" alias="TEST/A">
    <output>
        <file path="test-a.txt"/>
    </output>
</Service>
```

### Running DCU

To run `DCU`, provide the path to the configuration file as an argument:

```bash
/path-to-bin/dcu /path-to-config/config.xml
```
