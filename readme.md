## Description

IDA Discord RPC is a plugin for IDA that displays your current activity as Discord Rich Presence. It enhances your Discord profile with real-time information about what you're working on inside IDA.

## Compatible

Only tested with IDA Pro 9.1

## Building

Building is only supported on linux currently, but if you replace some file paths it does also build for windows.  
You need to take lib/discord_game_sdk/lib/x86_64/discord_game_sdk.so into your build directory, under the name "discord_game_sdk.soos".

## Installation

To install the plugin, place the compiled library file, along with "discord_game_sdk.soos", into the following directory:

Windows:
```
C:\Users\<YourUsername>\AppData\Roaming\Hex-Rays\IDA Pro\plugins
```
> Replace `<YourUsername>` with your actual Windows username.  
  
Linux:
```
/path/to/idapro/plugins
```

After restarting IDA, the plugin will run in the background.  
It does not appear in the plugin menu, as it is designed to be invisible during normal use.  

## License

This project is licensed under the [Apache License 2.0](LICENSE).  
You are free to use, modify, and distribute this software under the terms of the license.
