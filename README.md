# SideScroller Demo in Unreal Engine 5.2

Just messing around with Unreal Engine and C++ to create a Mario-like game.

![Game Image](Content/Assets/Images/SideScroller.png)

## Contributing

```mermaid
flowchart TD;
    A[Start] --> B[Make change in your non-main branch]
    B --> Z[Test the change]
    Z --> C{Does it work?}
    C -->|No| B
    C -->|Yes| D[Check it in]
    D --> E[Create Pull Request]
    E --> F[End]
```

### Getting Started

1. Clone the repo
2. Download IDE (Rider or VSCode)
3. Open the repo in the IDE
4. test build

### Making Changes

1. Add changes to a branch other than `main` (good practice to add a task number in the branch title)
2. once tested and working, make a pull request to main (see flowchart above)

## Testing

Testing can be done one of three ways:
1. Compile the code (`ctrl+alt+F11`)
2. Use the little `tetris-icon` button at the bottom-right in unreal editor
3. run the game 
   a. in the editor and test your functionality
   b. as standalone from powershell (can have multiple instances)
   c. packaged as `development` package and run the executable

### Packaging

using the packaging selections in UE (Unreal Editor), choose the proper platform for your dev machine.

### Running in PowerShell

example command:
```shell
 & F:\UE_5.2\Engine\Binaries\Win64\UnrealEditor.exe C:\Users\A-A-Ron\git\SideScroller\SideScroller.uproject /Game/Maps/Map_MainMenu -game -log -v -windowed resx=1280 resy=720
```

## Installing Unreal Editor

1. Download Epic Games Launcher for the inter-webs
2. Will have to sign up for an account
3. download and install Unreal Engine (for this project, we use 5.2.1)
4. crank up UE5 and direct it to the project
5. Hit the little `play` button to run the game
