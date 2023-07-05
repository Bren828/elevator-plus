# Elevator Plus

Creating an elevator in 5 minutes in SAMP

![Crosshair](https://raw.githubusercontent.com/Bren828/elevator-plus/main/preview.png)


## Main features
* Digital display
* Call queue
* Compatibility `mdialog`, `y_hooks`

## Reference
* [Installation](https://github.com/Bren828/elevator-plus#installation)
* [Example](https://github.com/Bren828/elevator-plus#example)
* [Functions](https://github.com/Bren828/elevator-plus#functions)
* [Callbacks](https://github.com/Bren828/elevator-plus#callbacks)
* [Definition](https://github.com/Bren828/elevator-plus#definition)


## Installation

Include in your code and begin using the library:
```pawn
#include <elevator-plus>
```

## Example

```pawn
public OnGameModeInit()
{
    // Create an elevator
    CreateElevator("ls_beach_building", 287.942504, -1609.334838, 33.827411, 0.0, 0.0, 80.0);

    // Create a floor
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,17.8593, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,32.9621, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,47.0014, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,51.5025, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,56.0036, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,60.5047, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,65.0058, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,69.5069, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,73.9923, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,78.4934, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,82.9945, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,87.4956, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,91.9967, 0.0);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,96.4978, 0.0);

    // Move the elevator to a floor
    SetElevatorFloor("ls_beach_building", 1);

    return 1;
}
```
## Functions

#### CreateElevator(const elevator_name[], Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
> Create an elevator
> * `elevator_name[]` - Elevator name
> * `Float:x` - The x coordinate to create the object
> * `Float:y` - The y coordinate to create the object
> * `Float:z` - The z coordinate to create the object
> * `Float:rx` - The rx rotation of the object
> * `Float:ry` - The ry rotation of the object
> * `Float:rz` - The rz rotation of the object


#### CreateElevatorFloor(const elevator_name[], Float:x, Float:y, Float:z, Float:door_height = 0.0, const text_dialog[] = "", const text_digital_display[] = "")
> Create a floor with a call button
> * `elevator_name[]` - Elevator name
> * `Float:x` - The x coordinate
> * `Float:y` - The y coordinate
> * `Float:z` - The z coordinate
> * `Float:door_height` - Door height
> * `text_dialog[]` - Name in the list of floors
> * `text_digital_display[]` - Displays the floor number


#### SetElevatorFloor(const elevator_name[], floor)
> Move the elevator to a floor
> * `elevator_name[]` - Elevator name
> * `floor` - Floor number


#### GetElevatorObjectID(const elevator_name[], &cabin_id, &doorL_id, &doorR_id, &digital_display_id)
> Get the elevator object id
> * `elevator_name[]` - Elevator name
> * `&cabin_id` - Elevator cab id
> * `&doorL_id` - Left door id
> * `&doorR_id` - Right door id
> * `&digital_display_id` - Digital display id
> * `Note` - **CreateObject is used**

#### GetElevatorFloorDoorObjectID(const elevator_name[], floor, &doorL_id, &doorR_id)
> Get the ID of the floor doors object
> * `elevator_name[]` - Elevator name
> * `floor` - Floor number
> * `&doorL_id` - Left door id
> * `&doorR_id` - Right door id
> * `Note` - **CreateDynamicObject is used**

## Callbacks

#### public OnElevatorMoveToFloor(const elevator_name[], old_floor, new_floor)
> Called when moving to a floor
> * `elevator_name[]` - Elevator name
> * `old_floor` - Old floor
> * `new_floor` - New floor

#### public OnElevatorArrivedFloor(const elevator_name[], floor)
> Called when the elevator arrives
> * `elevator_name[]` - Elevator name
> * `floor` - Floor number


## Definition

```pawn
#define ELEVATOR_MAX_ELEVATOR                   10

#define ELEVATOR_MAX_FLOORS                     30

#define ELEVATOR_MAX_NAME_LENGTH                32

#define ELEVATOR_DOORS_SPEED                    3.0 // movement speed of the doors

#define ELEVATOR_SPEED                          3.0 // movement speed of the elevator

#define ELEVATOR_UPDATE_TIMER                   900 // updating the digital display, door status

#define ELEVATOR_DOOR_CLOSING_TIME              8000 // milliseconds

#define ELEVATOR_OBJECT_DISTANCE                100.0

#define ELEVATOR_ENABLE_DIGITAL_DISPLAY         true

#define ELEVATOR_DIALOG_ID                      27303

static ELEVATOR_CABIN_TEXT[] =                  "{CCCCCC}~k~~SNEAK_ABOUT~";

static ELEVATOR_FLOOR_TEXT[] =                  "Call the elevator\n{CCCCCC}~k~~SNEAK_ABOUT~{FFFFFF}";

static ELEVATOR_FLOOR_IN_DIALOG_TEXT[] =        "Floor %d";

static ELEVATOR_TEXT_COMING_TO_YOU[] =          "{AFAFAF}Already coming to you";

static ELEVATOR_TEXT_ALREADY_IN_QUEUE[] =       "{AFAFAF}The floor is already in the queue";

static ELEVATOR_TEXT_CALLED[] =                 "Elevator called";

static ELEVATOR_FLOOR_COLOR_NOW[] =             "{65cd38}";
```

