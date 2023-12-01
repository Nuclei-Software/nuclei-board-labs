## Package Base Information
name: app-`appname`
owner: nuclei
version: 1.1.0
description: Nuclei `appdesc` lab
type: app
keywords:
  - nuclei board labs
category: Board Labs
license: Apache-2.0
homepage: `apphomepage`

## Package Dependency
dependencies:
  - name: `appdep`
    owner: nuclei
    version:

## Package Configurations
configuration:
  app_commonflags:
    value:`appflags`
    type: text
    description: Application Compile Flags

## Source Code Management
codemanage:
  copyfiles:
    - path: ["*.c", "*.h", "*.S", "*.s", "*.C"]
  incdirs:
    - path: ["./"]
  libdirs:
  ldlibs:
    - libs: ["m"]

## Build Configuration
buildconfig:
  - type: common
    common_flags: # flags need to be combined together across all packages
      - flags: ${app_commonflags}
    ldflags:
    cflags:
    asmflags:
    cxxflags:
    prebuild_steps: # could be override by app/bsp type
      command:
      description:
    postbuild_steps: # could be override by app/bsp type
      command:
      description:
