# `Va416x0::BuildInfo`

Provides information on the current build via events and telemetry. Build information includes:
1. Deployment name
   - ex. `BreadboardGds`
2. Build identifier
   - For user builds in development containers, this has the format `<date>-<branch>-dev-<username>`
   - For continuous integration builds, this has the format `<date>-<branch>-<ci_server>-<build_number>`
   - Dates are generated using `date +%Y-%m-%d-%H-%M-%S`, example output: `2025-07-14-21-15-35`
   - Note that for continuous integration builds, the date is snapshotted only once across all deployments, whereas for user builds it is snapshotted every time a deployment is linked

## Implementation

Build information is gathered as an auto-coding step during compilation. The information is pulled
from the CMake environment and used to produce source files that are linked to the deployment. This
is executed as part of the linker step to ensure that the build information is unique and specific
to each deployment that is being compiled.

The deployment name and build identifier are declared in the `<Va416x0/Svc/BuildInfo/BuildInfo.hpp>`
header. This defines the `DEPLOYMENT_NAME` and `BUILD_IDENTIFIER` string literals inside the
`Va416x0Svc::BuildInfo` namespace.

## Usage Examples

The `Va416x0.BuildInfo` component should be configured by a deployment by calling `configure`.

```cpp
    //! Set the build information for the current deployment and downlink initial events/telemetry
void configure();
```

The deployment should call `configure` inside its `initialize_deployment` function. This will
propagate the relevant information to the `Va416x0.BuildInfo` component and trigger the initial
events and telemetry to be downlinked.

Note that `Va416x0.BuildInfo` is intended to build on top of the information provided by the
`Svc.Version` component so deployments should include instances of both of these components.

### Diagrams

Add diagrams here

### Typical Usage

ex: using the `Va416x0.BuildInfo` component inside the `BreadboardGds` deployment

```cpp
void initialize_deployment() {
    // ...
    BreadboardGds::buildInfo.configure();
    // ...
}
```

## Class Diagram

Add a class diagram here

## Port Descriptions

No ports.

## Component States

`Va416x0.BuildInfo` is a passive component with no internal states.

## Sequence Diagrams

Add sequence diagrams here

## Parameters

None.

## Commands

| Name | Arguments | Description |
|---|---|---|
| `BUILD_INFO` | N/A | Produce events and telemetry with the requested information |

## Events

| Name | Message | Description |
|---|---|---|
| `DeploymentName` | `Deployment Name: {}` | Reports the deployment name |
| `BuildIdentifier` | `Build ID: {}` | Reports the build identifier, see above for further details |

## Telemetry

| Name | Type | Description |
|---|---|---|
| `DeploymentName` | string | Deployment name |
| `BuildIdentifier` | string | Build identifier, see above for further details |

## Unit Tests

Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
|---|---|---|---|

## Requirements

Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log

| Date | Description |
|---|---|
| 2025-07-14 | Initial Draft |
