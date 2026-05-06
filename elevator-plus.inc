/*
*
*
*
*   ---------------------------------------------------
*
*	Created by Bren828					
*									06 May 2026
*
*   Version: 1.3
*
*   ---------------------------------------------------
*
*
*	https://github.com/Bren828/elevator-plus
*
*
*/


#if defined _elevator_included 
	#endinput
#endif
#define _elevator_included


#if !defined ELEVATOR_MAX_ELEVATORS
    #define ELEVATOR_MAX_ELEVATORS 10
#endif

#if !defined ELEVATOR_MAX_FLOORS
    #define ELEVATOR_MAX_FLOORS 30
#endif

#if !defined ELEVATOR_NAME_LENGTH
    #define ELEVATOR_NAME_LENGTH 32
#endif

#if !defined ELEVATOR_DOOR_SPEED
    #define ELEVATOR_DOOR_SPEED 3.0 // movement speed of the doors
#endif

#if !defined ELEVATOR_CABIN_SPEED
    #define ELEVATOR_CABIN_SPEED 3.0 // movement speed of the elevator
#endif

#if !defined ELEVATOR_TIMER_INTERVAL
    #define ELEVATOR_TIMER_INTERVAL 500 
#endif

#if !defined ELEVATOR_DOOR_CLOSE_DELAY
    #define ELEVATOR_DOOR_CLOSE_DELAY 8000 // milliseconds
#endif

#if !defined ELEVATOR_OBJECT_DISTANCE
    #define ELEVATOR_OBJECT_DISTANCE 100.0
#endif

#if !defined ELEVATOR_DISABLE_DISPLAY
    //#define ELEVATOR_DISABLE_DISPLAY
#endif

#if !defined ELEVATOR_DIALOG_ID
    #define ELEVATOR_DIALOG_ID 27303
#endif

#if !defined ELEVATOR_CHAT_MESSAGE_COLOR
    #define ELEVATOR_CHAT_MESSAGE_COLOR 0xFFFFFFAA
#endif

#if !defined ELEVATOR_TEXT_CABIN
    static const ELEVATOR_TEXT_CABIN[] = "{CCCCCC}~k~~SNEAK_ABOUT~";
#endif

#if !defined ELEVATOR_TEXT_FLOOR_CALL
    static const ELEVATOR_TEXT_FLOOR_CALL[] = "Call the elevator\n{CCCCCC}~k~~SNEAK_ABOUT~{FFFFFF}";
#endif

#if !defined ELEVATOR_TEXT_FLOOR_DIALOG
    static const ELEVATOR_TEXT_FLOOR_DIALOG[] = "Floor %d";
#endif

#if !defined ELEVATOR_TEXT_COMING
    static const ELEVATOR_TEXT_COMING[] = "{AFAFAF}Already coming to you";
#endif

#if !defined ELEVATOR_TEXT_QUEUED
    static const ELEVATOR_TEXT_QUEUED[] = "{AFAFAF}The floor is already in the queue";
#endif

#if !defined ELEVATOR_TEXT_CALLED
    static const ELEVATOR_TEXT_CALLED[] = "Elevator called";
#endif

#if !defined ELEVATOR_TEXT_FLOOR_COLOR
    static const ELEVATOR_TEXT_FLOOR_COLOR[] = "{65cd38}";
#endif

#if !defined ELEVATOR_DIALOG_TITLE
    static const ELEVATOR_DIALOG_TITLE[] = "Select floor";
#endif

#if !defined ELEVATOR_DIALOG_BUTTON1
    static const ELEVATOR_DIALOG_BUTTON1[] = "Select";
#endif

#if !defined ELEVATOR_DIALOG_BUTTON2
    static const ELEVATOR_DIALOG_BUTTON2[] = "Close";
#endif


enum E_ELEVATOR {

    bool:e_Created,
    e_Name[ELEVATOR_NAME_LENGTH],
    e_DoorStatus,
    e_MaxFloor,
    e_TargetFloor,
    e_LastFloor,
    e_ClosingTime,
    e_NextFloor,
    e_MoveDelay,
    e_DisplayUpdates,

    e_QueueFloors[ELEVATOR_MAX_FLOORS],
    e_Timer,
    e_World,
    e_Interior,

//Cabin
    e_CabinObject,
    e_CabinDisplayObject,
    
    Float:e_CabinDoorRX, 
    Float:e_CabinDoorRY, 
    Float:e_CabinDoorRZ,
    
    //Left Door 
    e_LeftCabinDoor,
    Float:e_ClosedLeftCabinDoor_X,
    Float:e_ClosedLeftCabinDoor_Y,
    Float:e_OpenLeftCabinDoor_X,
    Float:e_OpenLeftCabinDoor_Y,

    //Right Door 
    e_RightCabinDoor,
    Float:e_ClosedRightCabinDoor_X,
    Float:e_ClosedRightCabinDoor_Y,
    Float:e_OpenRightCabinDoor_X,
    Float:e_OpenRightCabinDoor_Y,

    //Text3D
    Text3D:e_CabinText3D,
    Float:e_CabinText3D_X,
    Float:e_CabinText3D_Y,
    Float:e_CabinText3D_Z,
}

static elevator[ELEVATOR_MAX_ELEVATORS][E_ELEVATOR];
static max_elevator;


enum E_ELEVATOR_FLOORS {

    Float:ef_ZOffset,
    Float:ef_DoorZ,

    //Text3D
    Text3D:ef_Text3D,
    Float:ef_Text3D_X,
    Float:ef_Text3D_Y,
    Float:ef_Text3D_Z,

    //Left Door
    ef_LeftDoorObject,
    Float:ef_ClosedLeftDoor_X,
    Float:ef_ClosedLeftDoor_Y,
    Float:ef_OpenLeftDoor_X,
    Float:ef_OpenLeftDoor_Y,

    //Right Door
    ef_RightDoorObject,
    Float:ef_ClosedRightDoor_X,
    Float:ef_ClosedRightDoor_Y,
    Float:ef_OpenRightDoor_X,
    Float:ef_OpenRightDoor_Y
}

static elevatorFloors[ELEVATOR_MAX_ELEVATORS][ELEVATOR_MAX_FLOORS][E_ELEVATOR_FLOORS];


static elevatorTextDialog[ELEVATOR_MAX_ELEVATORS][ELEVATOR_MAX_FLOORS][ELEVATOR_NAME_LENGTH];
static elevatorDisplayText[ELEVATOR_MAX_ELEVATORS][ELEVATOR_MAX_FLOORS][ELEVATOR_NAME_LENGTH];

static playerElevatorID[MAX_PLAYERS];


enum (<<=1) {
    ELEVATOR_DOOR_CLOSED = 0,
    ELEVATOR_DOOR_OPEN = 1,
    ELEVATOR_DOOR_CLOSING = 2,
    ELEVATOR_DOOR_OPENS = 3
}


stock CreateElevator(const elevatorName[], Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid = -1, interiorid = -1) {

    if(max_elevator >= ELEVATOR_MAX_ELEVATORS) {
        print("elevator --->>> | CreateElevator | The maximum number of elevators has been reached.");
        return 0;
    }

    elevator[max_elevator][e_Created] = true;
    format(elevator[max_elevator][e_Name], ELEVATOR_NAME_LENGTH, elevatorName);

    elevator[max_elevator][e_CabinObject] = CreateObject(18755, x, y, z - 0.06, rx, ry, rz, ELEVATOR_OBJECT_DISTANCE);

    #if !defined ELEVATOR_DISABLE_DISPLAY
        SetObjectMaterial(elevator[max_elevator][e_CabinObject], 4, -1, "none", "none", 0xFF000000);
        E_CreateCabinDisplay(max_elevator);
    #endif
  
    elevator[max_elevator][e_CabinDoorRX] = rx;
    elevator[max_elevator][e_CabinDoorRY] = ry;
    elevator[max_elevator][e_CabinDoorRZ] = rz;

    elevator[max_elevator][e_CabinText3D] = Text3D:INVALID_3DTEXT_ID;
    elevator[max_elevator][e_DoorStatus] = ELEVATOR_DOOR_CLOSED;
    elevator[max_elevator][e_MaxFloor] = 0;
    elevator[max_elevator][e_TargetFloor] = 0;
    elevator[max_elevator][e_LastFloor] = 0;
    elevator[max_elevator][e_ClosingTime] = 0;
    elevator[max_elevator][e_NextFloor] = -1;
    elevator[max_elevator][e_Timer] = 0;
    elevator[max_elevator][e_World] = worldid;
    elevator[max_elevator][e_Interior] = interiorid;


//left door
    E_ApplyOffset(x, y, z, rz,
        elevator[max_elevator][e_ClosedLeftCabinDoor_X], 
        elevator[max_elevator][e_ClosedLeftCabinDoor_Y], 
        z, 
        .xOffset = 0.0, .yOffset = 0.01, .zOffset = 0.0);

    elevator[max_elevator][e_LeftCabinDoor] = CreateDynamicObject(18757, 
        elevator[max_elevator][e_ClosedLeftCabinDoor_X], elevator[max_elevator][e_ClosedLeftCabinDoor_Y], z, 
        rx, ry, rz, 
        worldid, interiorid,
        -1, 
        ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);
   
    //left open door
    E_ApplyOffset(x, y, z, rz,
        elevator[max_elevator][e_OpenLeftCabinDoor_X], 
        elevator[max_elevator][e_OpenLeftCabinDoor_Y], 
        z, 
        .xOffset = 0.0, .yOffset = 1.6, .zOffset = 0.0);


//right door
    E_ApplyOffset(x, y, z, rz,
        elevator[max_elevator][e_ClosedRightCabinDoor_X], 
        elevator[max_elevator][e_ClosedRightCabinDoor_Y], 
        z, 
        .xOffset = 0.0, .yOffset = 0.01, .zOffset = 0.0);

    elevator[max_elevator][e_RightCabinDoor] = CreateDynamicObject(18756, 
        elevator[max_elevator][e_ClosedRightCabinDoor_X], elevator[max_elevator][e_ClosedRightCabinDoor_Y], z, 
        rx, ry, rz, 
        worldid, interiorid, 
        -1, 
        ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);

    //right open door
    E_ApplyOffset(x, y, z, rz,
        elevator[max_elevator][e_OpenRightCabinDoor_X], 
        elevator[max_elevator][e_OpenRightCabinDoor_Y], 
        z, 
        .xOffset = 0.0, .yOffset = -1.6, .zOffset = 0.0);


//3DText
    E_ApplyOffset(x, y, z, rz,
        elevator[max_elevator][e_CabinText3D_X], 
        elevator[max_elevator][e_CabinText3D_Y], 
        elevator[max_elevator][e_CabinText3D_Z], 
        .xOffset = -1.6, .yOffset = -1.85, .zOffset = -0.5);

    max_elevator ++;
    return 1;
}



stock CreateElevatorFloor(const elevatorName[], Float:x, Float:y, Float:z, Float:z_offset = 0.0, const dialogText[] = "", const displayText[] = "") {

    new elevatorid = -1;

    for(new i; i < max_elevator; i ++) {

        if(!elevator[i][e_Created]) {
            continue;
        }

        if(strcmp(elevatorName, elevator[i][e_Name], false)) {
            continue;
        }

        elevatorid = i;
        break;
    }

    if(elevatorid == -1) {
        return 0;
    }

    new max_floor = elevator[elevatorid][e_MaxFloor];

    elevator[elevatorid][e_MaxFloor] ++;
    elevatorFloors[elevatorid][max_floor][ef_ZOffset] = z_offset + 0.94;
    elevatorFloors[elevatorid][max_floor][ef_DoorZ] = z;

//dialog
    if(strlen(dialogText) >= 1) {
        format(elevatorTextDialog[elevatorid][max_floor], ELEVATOR_NAME_LENGTH, dialogText);
    }
    else {
        format(elevatorTextDialog[elevatorid][max_floor], ELEVATOR_NAME_LENGTH, ELEVATOR_TEXT_FLOOR_DIALOG, max_floor + 1);
    }


//display
    if(strlen(displayText) >= 1) {
        format(elevatorDisplayText[elevatorid][max_floor], ELEVATOR_NAME_LENGTH, displayText);
    }
    else {
        format(elevatorDisplayText[elevatorid][max_floor], ELEVATOR_NAME_LENGTH, "%d", max_floor + 1);
    }


//3DText
    elevatorFloors[elevatorid][max_floor][ef_Text3D_X] = x;
    elevatorFloors[elevatorid][max_floor][ef_Text3D_Y] = y;
    elevatorFloors[elevatorid][max_floor][ef_Text3D_Z] = z;

    elevatorFloors[elevatorid][max_floor][ef_Text3D] = CreateDynamic3DTextLabel(ELEVATOR_TEXT_FLOOR_CALL, -1, 
        x, y, z, 
        10.0, 
        INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 
        1, 
        elevator[elevatorid][e_World], elevator[elevatorid][e_Interior], 
        -1, 10.0);
    

//left closed door
    E_ApplyOffset(
        elevator[elevatorid][e_ClosedLeftCabinDoor_X], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_Y], 
        elevatorFloors[elevatorid][max_floor][ef_DoorZ], 
        elevator[elevatorid][e_CabinDoorRZ],

        elevatorFloors[elevatorid][max_floor][ef_ClosedLeftDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_ClosedLeftDoor_Y], 
        z, 
        .xOffset = -0.29, .yOffset = -0.01, .zOffset = 0.0);

    elevatorFloors[elevatorid][max_floor][ef_LeftDoorObject] = CreateDynamicObject(18757, 
        elevatorFloors[elevatorid][max_floor][ef_ClosedLeftDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_ClosedLeftDoor_Y], 
        z + elevatorFloors[elevatorid][max_floor][ef_ZOffset],
        elevator[elevatorid][e_CabinDoorRX], 
        elevator[elevatorid][e_CabinDoorRY], 
        elevator[elevatorid][e_CabinDoorRZ], 
        elevator[elevatorid][e_World], elevator[elevatorid][e_Interior], 
        -1, 
        ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);
    
    //left open door
    E_ApplyOffset(
        elevator[elevatorid][e_ClosedLeftCabinDoor_X], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_Y], 
        elevatorFloors[elevatorid][max_floor][ef_DoorZ], 
        elevator[elevatorid][e_CabinDoorRZ],

        elevatorFloors[elevatorid][max_floor][ef_OpenLeftDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_OpenLeftDoor_Y], 
        z, 
        .xOffset = -0.29, .yOffset = 1.6, .zOffset = 0.0);


//right closed door
    E_ApplyOffset(
        elevator[elevatorid][e_ClosedRightCabinDoor_X], 
        elevator[elevatorid][e_ClosedRightCabinDoor_Y], 
        elevatorFloors[elevatorid][max_floor][ef_DoorZ], 
        elevator[elevatorid][e_CabinDoorRZ],

        elevatorFloors[elevatorid][max_floor][ef_ClosedRightDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_ClosedRightDoor_Y], 
        z, 
        .xOffset = -0.29, .yOffset = -0.01, .zOffset = 0.0);

    elevatorFloors[elevatorid][max_floor][ef_RightDoorObject] = CreateDynamicObject(18756, 
        elevatorFloors[elevatorid][max_floor][ef_ClosedRightDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_ClosedRightDoor_Y], 
        z + elevatorFloors[elevatorid][max_floor][ef_ZOffset],
        elevator[elevatorid][e_CabinDoorRX], 
        elevator[elevatorid][e_CabinDoorRY], 
        elevator[elevatorid][e_CabinDoorRZ], 
        elevator[elevatorid][e_World], elevator[elevatorid][e_Interior], 
        -1, 
        ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);
    
    //right open door
    E_ApplyOffset(
        elevator[elevatorid][e_ClosedLeftCabinDoor_X], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_Y], 
        elevatorFloors[elevatorid][max_floor][ef_DoorZ], 
        elevator[elevatorid][e_CabinDoorRZ],

        elevatorFloors[elevatorid][max_floor][ef_OpenRightDoor_X], 
        elevatorFloors[elevatorid][max_floor][ef_OpenRightDoor_Y], 
        z, 
        .xOffset = -0.29, .yOffset = -1.6, .zOffset = 0.0);

    
// moving the elevator to the first floor
    if(max_floor == 0) { 
    
        elevator[elevatorid][e_TargetFloor] = 0;
        elevator[elevatorid][e_LastFloor] = 0;
        elevator[elevatorid][e_CabinText3D_Z] = elevatorFloors[elevatorid][0][ef_DoorZ] + elevatorFloors[elevatorid][0][ef_ZOffset];

        //Cabin
        new Float:MX, Float:MY, Float:MZ;
        GetObjectPos(elevator[elevatorid][e_CabinObject], MX, MY, MZ);
        SetObjectPos(elevator[elevatorid][e_CabinObject], MX, MY, elevator[elevatorid][e_CabinText3D_Z] - 0.06);

        GetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], MX, MY, MZ);
        SetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], MX, MY, elevator[elevatorid][e_CabinText3D_Z]);

        GetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], MX, MY, MZ);
        SetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], MX, MY, elevator[elevatorid][e_CabinText3D_Z]);
    
        //Text3D
        E_CreateCabinText3D(elevatorid);

        //Display
        #if !defined ELEVATOR_DISABLE_DISPLAY
            SetObjectMaterialText(elevator[elevatorid][e_CabinDisplayObject], 
                elevatorDisplayText[elevatorid][0], 
                0, 80, 
                "Arial Black", 60, 1, 0xFFFFFFFF, 0x00000000, 1);
        #endif
    }

    return 1;
}


stock SetElevatorFloor(const elevatorName[], floor) {

    if(max_elevator == 0 || floor < 0 || floor >= ELEVATOR_MAX_FLOORS) {
        return 0;
    }

    new elevatorid = -1;

    for(new i; i < max_elevator; i ++) {

        if(!elevator[i][e_Created]) {
            continue;
        }

        if(strcmp(elevatorName, elevator[i][e_Name], false)) {
            continue;
        }

        if(elevator[i][e_MaxFloor] == 0 || floor >= elevator[i][e_MaxFloor] || floor == elevator[i][e_TargetFloor]) {
            break;
        }

        elevatorid = i;
        break;
    }

    if(elevatorid == -1) {
        return 0;
    }

    new Float:X, Float:Y, Float:Z;

    elevator[elevatorid][e_TargetFloor] = floor;
    elevator[elevatorid][e_LastFloor] = floor;
    elevator[elevatorid][e_CabinText3D_Z] = elevatorFloors[elevatorid][floor][ef_DoorZ] + elevatorFloors[elevatorid][floor][ef_ZOffset];

//Cabin
    GetObjectPos(elevator[elevatorid][e_CabinObject], X, Y, Z);
    SetObjectPos(elevator[elevatorid][e_CabinObject], X, Y, elevator[elevatorid][e_CabinText3D_Z] - 0.06);

    GetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], X, Y, Z);
    SetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], X, Y, elevator[elevatorid][e_CabinText3D_Z]);

    GetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], X, Y, Z);
    SetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], X, Y, elevator[elevatorid][e_CabinText3D_Z]);


//Text3D
    E_CreateCabinText3D(elevatorid);
    

//Display
    #if !defined ELEVATOR_DISABLE_DISPLAY
        // FIXED: Text does not update after 16 calls against the same object.
        elevator[elevatorid][e_DisplayUpdates] ++;

        if(elevator[elevatorid][e_DisplayUpdates] >= 6) {
            E_CreateCabinDisplay(elevatorid);
        }

        SetObjectMaterialText(elevator[elevatorid][e_CabinDisplayObject], 
            elevatorDisplayText[elevatorid][floor], 
            0, 80, 
            "Arial Black", 
            60, 1, 0xFFFFFFFF, 0x00000000, 1);    
    #endif

    return 1;
}


stock GetElevatorCabinObjectID(const elevatorName[], &cabinID, &leftDoorID, &rightDoorID, &displayID) {

    if(max_elevator == 0) {
        return 0;
    }

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++) {

        if(!elevator[elevatorid][e_Created]) {
            continue;
        }

        if(strcmp(elevatorName, elevator[elevatorid][e_Name], false)) {
            continue;
        }

        cabinID = elevator[elevatorid][e_CabinObject];
        displayID = elevator[elevatorid][e_CabinDisplayObject];

        leftDoorID = elevator[elevatorid][e_LeftCabinDoor];
        rightDoorID = elevator[elevatorid][e_RightCabinDoor];
        return 1;
    }

    return 0;
}


stock GetElevatorFloorDoorID(const elevatorName[], floor, &leftDoorID, &rightDoorID) {

    if(max_elevator == 0 || floor < 0 || floor >= ELEVATOR_MAX_FLOORS) {
        return 0;
    }

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++) {
        
        if(!elevator[elevatorid][e_Created]) {
            continue;
        }

        if(strcmp(elevatorName, elevator[elevatorid][e_Name], false)) {
            continue;
        }

        if(floor >= elevator[elevatorid][e_MaxFloor]) {
            return 1;
        }

        leftDoorID = elevatorFloors[elevatorid][floor][ef_LeftDoorObject];
        rightDoorID = elevatorFloors[elevatorid][floor][ef_RightDoorObject];

        return 1;
    }

    return 0;
}


static stock OpenElevatorDoors(elevatorid, floorid) {

    new Float:X, Float:Y, Float:Z;

//Cabin
    //Left Door
    GetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_LeftCabinDoor], 
        elevator[elevatorid][e_OpenLeftCabinDoor_X], 
        elevator[elevatorid][e_OpenLeftCabinDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);

    //Right Door
    GetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_RightCabinDoor], 
        elevator[elevatorid][e_OpenRightCabinDoor_X], 
        elevator[elevatorid][e_OpenRightCabinDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);

//Floor
    //Left Door
    GetDynamicObjectPos(elevatorFloors[elevatorid][floorid][ef_LeftDoorObject], X, Y, Z);
    MoveDynamicObject(elevatorFloors[elevatorid][floorid][ef_LeftDoorObject], 
        elevatorFloors[elevatorid][floorid][ef_OpenLeftDoor_X], 
        elevatorFloors[elevatorid][floorid][ef_OpenLeftDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);

    //Right Door
    GetDynamicObjectPos(elevatorFloors[elevatorid][floorid][ef_RightDoorObject], X, Y, Z);
    MoveDynamicObject(elevatorFloors[elevatorid][floorid][ef_RightDoorObject], 
        elevatorFloors[elevatorid][floorid][ef_OpenRightDoor_X], 
        elevatorFloors[elevatorid][floorid][ef_OpenRightDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);


    elevator[elevatorid][e_ClosingTime] = GetTickCount() + ELEVATOR_DOOR_CLOSE_DELAY;
    elevator[elevatorid][e_DoorStatus] = ELEVATOR_DOOR_OPENS;
    return 1;
}


static stock ClosedElevatorDoors(elevatorid, floorid) {

    if(elevator[elevatorid][e_DoorStatus] == ELEVATOR_DOOR_CLOSED) {
        elevator[elevatorid][e_MoveDelay] = GetTickCount() + 2000;
        return 0;
    }

    new Float:X, Float:Y, Float:Z;

//Cabin
    //DoorL
    GetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_LeftCabinDoor], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_X], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);

    //DoorR
    GetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_RightCabinDoor], 
        elevator[elevatorid][e_ClosedRightCabinDoor_X], 
        elevator[elevatorid][e_ClosedRightCabinDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);
    
//Floor
    //DoorL
    GetDynamicObjectPos(elevatorFloors[elevatorid][floorid][ef_LeftDoorObject], X, Y, Z);
    MoveDynamicObject(elevatorFloors[elevatorid][floorid][ef_LeftDoorObject], 
        elevatorFloors[elevatorid][floorid][ef_ClosedLeftDoor_X], 
        elevatorFloors[elevatorid][floorid][ef_ClosedLeftDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);

    //DoorR
    GetDynamicObjectPos(elevatorFloors[elevatorid][floorid][ef_RightDoorObject], X, Y, Z);
    MoveDynamicObject(elevatorFloors[elevatorid][floorid][ef_RightDoorObject], 
        elevatorFloors[elevatorid][floorid][ef_ClosedRightDoor_X], 
        elevatorFloors[elevatorid][floorid][ef_ClosedRightDoor_Y], 
        Z, 
        ELEVATOR_DOOR_SPEED);


    elevator[elevatorid][e_ClosingTime] = 0;
    elevator[elevatorid][e_DoorStatus] = ELEVATOR_DOOR_CLOSING;
    return 1;
}


static stock ElevatorMoveToFloor(elevatorid, next_floor) {

    new Float:X, Float:Y, Float:Z;

    //Cabin
    GetObjectPos(elevator[elevatorid][e_CabinObject], X, Y, Z);
    MoveObject(elevator[elevatorid][e_CabinObject], 
        X, 
        Y, 
        elevatorFloors[elevatorid][next_floor][ef_DoorZ] + elevatorFloors[elevatorid][next_floor][ef_ZOffset] - 0.06, 
        ELEVATOR_CABIN_SPEED);
    
    //DoorL
    GetDynamicObjectPos(elevator[elevatorid][e_LeftCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_LeftCabinDoor], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_X], 
        elevator[elevatorid][e_ClosedLeftCabinDoor_Y], 
        elevatorFloors[elevatorid][next_floor][ef_DoorZ] + elevatorFloors[elevatorid][next_floor][ef_ZOffset], 
        ELEVATOR_CABIN_SPEED);

    //DoorR
    GetDynamicObjectPos(elevator[elevatorid][e_RightCabinDoor], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][e_RightCabinDoor], 
        elevator[elevatorid][e_ClosedRightCabinDoor_X], 
        elevator[elevatorid][e_ClosedRightCabinDoor_Y], 
        elevatorFloors[elevatorid][next_floor][ef_DoorZ] + elevatorFloors[elevatorid][next_floor][ef_ZOffset], 
        ELEVATOR_CABIN_SPEED);


    if(IsValidDynamic3DTextLabel(elevator[elevatorid][e_CabinText3D])) {
        DestroyDynamic3DTextLabel(elevator[elevatorid][e_CabinText3D]);
        elevator[elevatorid][e_CabinText3D] = Text3D:INVALID_3DTEXT_ID;
    }

    elevator[elevatorid][e_CabinText3D_Z] = elevatorFloors[elevatorid][next_floor][ef_DoorZ] + elevatorFloors[elevatorid][next_floor][ef_ZOffset];
    return 1;
}


static stock IsFloorInQueue(elevatorid, floor) {

    for(new i; i < elevator[elevatorid][e_MaxFloor]; i ++) {

        if(elevator[elevatorid][e_QueueFloors][i] == 0) {
            continue;
        }

        if(elevator[elevatorid][e_QueueFloors][i] != floor + 1) {
            continue;
        }

        return 1;
    }

    return 0;
}


static stock RemoveFirstQueueFloor(elevatorid) {

    new max_queue, 
        floor[ELEVATOR_MAX_FLOORS];

    for(new i; i < elevator[elevatorid][e_MaxFloor]; i ++) {

        if(!elevator[elevatorid][e_QueueFloors][i]) {
            continue;
        }

        floor[max_queue] = elevator[elevatorid][e_QueueFloors][i];
        elevator[elevatorid][e_QueueFloors][i] = 0;

        max_queue++;
    }

    for(new i; i < max_queue; i ++) {
        elevator[elevatorid][e_QueueFloors][i] = floor[i];
    }

    return 1;
}


#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#else
    public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#endif
{
    if(newkeys & KEY_WALK && max_elevator >= 1) {

        new elevatorid = -1,
            floor,
            bool:inElevator;
            
        for(new i; i < max_elevator; i ++) {

            if(!elevator[i][e_Created]) {
                continue;
            }

            if(!elevator[i][e_MaxFloor]) {
                continue;
            }

            if(elevator[i][e_World] != -1 && elevator[i][e_World] != GetPlayerVirtualWorld(playerid)) {
                continue; 
            }

            if(elevator[i][e_Interior] != -1 && elevator[i][e_Interior] != GetPlayerInterior(playerid)) {
                continue; 
            }

            if(IsPlayerInRangeOfPoint(playerid, 1.1, elevator[i][e_CabinText3D_X], elevator[i][e_CabinText3D_Y], elevator[i][e_CabinText3D_Z])) {

                if(elevator[i][e_NextFloor] != -1) {
                    break; 
                }

                elevatorid = i;
                inElevator = true;
                break;
            }

            for(new floor_id; floor_id < elevator[i][e_MaxFloor]; floor_id ++) {

                if(!IsPlayerInRangeOfPoint(playerid, 1.0, 
                    elevatorFloors[i][floor_id][ef_Text3D_X], elevatorFloors[i][floor_id][ef_Text3D_Y], elevatorFloors[i][floor_id][ef_Text3D_Z])) {
                    continue;
                }
                
                elevatorid = i;
                floor = floor_id;
                inElevator = false;
                break;
            }

            if(elevatorid != -1) {
                break;
            }
        }

        if(elevatorid != -1) {

            if(inElevator) {

                if(elevator[elevatorid][e_NextFloor] != -1) {
                    return 1;
                }

                if(IsDynamicObjectMoving(elevator[elevatorid][e_LeftCabinDoor])) {
                    return 1;
                }

                playerElevatorID[playerid] = elevatorid;
                
                new ep_string[((8 + ELEVATOR_NAME_LENGTH) * ELEVATOR_MAX_FLOORS) + sizeof(ELEVATOR_TEXT_FLOOR_COLOR)];

                for(new i; i < elevator[elevatorid][e_MaxFloor]; i ++) {
                    format(ep_string, sizeof(ep_string), "%s%s%s\n", 
                        ep_string, 
                        (elevator[elevatorid][e_TargetFloor] == i) ? (ELEVATOR_TEXT_FLOOR_COLOR) : (""), 
                        elevatorTextDialog[elevatorid][i]);
                }

                #if defined _mdialog_included
                    Dialog_Open(playerid, Dialog:elevator_floors, DIALOG_STYLE_LIST, ELEVATOR_DIALOG_TITLE, ep_string, ELEVATOR_DIALOG_BUTTON1, ELEVATOR_DIALOG_BUTTON2);
                #else
                    ShowPlayerDialog(playerid, ELEVATOR_DIALOG_ID, DIALOG_STYLE_LIST, ELEVATOR_DIALOG_TITLE, ep_string, ELEVATOR_DIALOG_BUTTON1, ELEVATOR_DIALOG_BUTTON2);
                #endif
                return 1;
            }


            if(elevator[elevatorid][e_NextFloor] != -1 || elevator[elevatorid][e_ClosingTime] != 0 && elevator[elevatorid][e_TargetFloor] != floor) {

                if(elevator[elevatorid][e_NextFloor] == floor) {
                    SendClientMessage(playerid, ELEVATOR_CHAT_MESSAGE_COLOR, ELEVATOR_TEXT_COMING);
                    return 1;
                }

                if(IsFloorInQueue(elevatorid, floor)) {
                    SendClientMessage(playerid, ELEVATOR_CHAT_MESSAGE_COLOR, ELEVATOR_TEXT_QUEUED);
                    return 1;
                }

                for(new i; i < elevator[elevatorid][e_MaxFloor]; i ++) {

                    if(elevator[elevatorid][e_QueueFloors][i] != 0) {
                        continue;
                    }

                    elevator[elevatorid][e_QueueFloors][i] = floor + 1;
                    break;
                }

                SendClientMessage(playerid, ELEVATOR_CHAT_MESSAGE_COLOR, ELEVATOR_TEXT_CALLED);
                return 1;
            }

            if(elevator[elevatorid][e_DoorStatus] != ELEVATOR_DOOR_CLOSED) {
                return 1;
            }

            if(!elevator[elevatorid][e_Timer]) {
                elevator[elevatorid][e_Timer] = SetTimerEx("on_elevator_timer", ELEVATOR_TIMER_INTERVAL, true, "d", elevatorid);
            }

            if(elevator[elevatorid][e_TargetFloor] == floor) {
                OpenElevatorDoors(elevatorid, floor);
            }
            else {
                elevator[elevatorid][e_NextFloor] = floor;
                ClosedElevatorDoors(elevatorid, elevator[elevatorid][e_TargetFloor]);

                SendClientMessage(playerid, ELEVATOR_CHAT_MESSAGE_COLOR, ELEVATOR_TEXT_CALLED);
            }
            return 1;
        }
    }

    #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerKeyStateChange_elevator
		return OnPlayerKeyStateChange_elevator(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}

#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnPlayerKeyStateChange
		#undef OnPlayerKeyStateChange
	#else
		#define _ALS_OnPlayerKeyStateChange
	#endif
	#define OnPlayerKeyStateChange OnPlayerKeyStateChange_elevator
	#if defined OnPlayerKeyStateChange_elevator
		forward OnPlayerKeyStateChange_elevator(playerid, newkeys, oldkeys);
	#endif
#endif



#if defined _mdialog_included
    DialogResponse:elevator_floors(playerid, response, listitem, inputtext[])
#else

    #if defined _inc_y_hooks || defined _INC_y_hooks
        hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
    #else
        public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
    #endif

#endif
{
    #if !defined _mdialog_included
        if(dialogid == ELEVATOR_DIALOG_ID)
        {
    #endif
            if(!response || listitem < 0 || listitem >= ELEVATOR_MAX_FLOORS) {
                return 1;
            }

            new elevatorid = playerElevatorID[playerid];

            if(elevator[elevatorid][e_NextFloor] != -1) {
                return 1;
            }

            if(IsDynamicObjectMoving(elevator[elevatorid][e_LeftCabinDoor])) {
                return 1;
            }

            if(!elevator[elevatorid][e_Timer]) {
                elevator[elevatorid][e_Timer] = SetTimerEx("on_elevator_timer", ELEVATOR_TIMER_INTERVAL, true, "d", elevatorid);    
            }

            if(elevator[elevatorid][e_TargetFloor] == listitem) {
                if(elevator[elevatorid][e_DoorStatus] == ELEVATOR_DOOR_CLOSED) {
                    OpenElevatorDoors(elevatorid, elevator[elevatorid][e_TargetFloor]);
                }
                return 1;
            }

            elevator[elevatorid][e_NextFloor] = listitem;
            ClosedElevatorDoors(elevatorid, elevator[elevatorid][e_TargetFloor]);
            return 1;

    #if !defined _mdialog_included
        }
        #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnDialogResponse_elevator
            return OnDialogResponse_elevator(playerid, dialogid, response, listitem, inputtext);
        #else 
            return 1;
        #endif
    #endif   
}

#if !defined _mdialog_included && !defined _inc_y_hooks && !defined _INC_y_hooks
    #if defined _ALS_OnDialogResponse
        #undef OnDialogResponse
    #else
        #define _ALS_OnDialogResponse
    #endif

    #define OnDialogResponse OnDialogResponse_elevator
    #if defined OnDialogResponse_elevator
        forward OnDialogResponse_elevator(playerid, dialogid, response, listitem, inputtext[]);
    #endif 
#endif



#if defined _inc_y_hooks || defined _INC_y_hooks
    hook OnDynamicObjectMoved(objectid)
#else
    public OnDynamicObjectMoved(objectid)
#endif
{
    if(max_elevator >= 1) {

        for(new elevatorid; elevatorid < max_elevator; elevatorid ++) {

            if(!elevator[elevatorid][e_Created]) {
                continue;
            }

            if(elevator[elevatorid][e_LeftCabinDoor] != objectid) {
                continue;
            }

            if(elevator[elevatorid][e_DoorStatus] == ELEVATOR_DOOR_CLOSED && elevator[elevatorid][e_NextFloor] != -1) {

                // the elevator arrived

                elevator[elevatorid][e_MoveDelay] = GetTickCount() + 2000;  
                elevator[elevatorid][e_TargetFloor] = elevator[elevatorid][e_NextFloor];

                if(elevator[elevatorid][e_QueueFloors][0] != 0) {

                    for(new i; i < elevator[elevatorid][e_MaxFloor]; i ++) {

                        if(elevator[elevatorid][e_QueueFloors][i] == 0) {
                            continue;
                        }

                        if(elevator[elevatorid][e_QueueFloors][i] != elevator[elevatorid][e_TargetFloor] + 1) {
                            continue;
                        }

                        elevator[elevatorid][e_QueueFloors][i] = 0;
                        RemoveFirstQueueFloor(elevatorid);
                        break;
                    }
                }

                #if defined OnElevatorArrivedFloor
                    OnElevatorArrivedFloor(elevator[elevatorid][e_Name], elevator[elevatorid][e_TargetFloor]);
                #endif
            }
            else if(elevator[elevatorid][e_DoorStatus] == ELEVATOR_DOOR_CLOSING) {

                elevator[elevatorid][e_DoorStatus] = ELEVATOR_DOOR_CLOSED;

                if(elevator[elevatorid][e_NextFloor] != -1) {
                    // We send the elevator
                    elevator[elevatorid][e_MoveDelay] = GetTickCount() + 2000;
                }
                else { // check queue
               
                    if(elevator[elevatorid][e_QueueFloors][0] != 0) {
                        elevator[elevatorid][e_NextFloor] = elevator[elevatorid][e_QueueFloors][0] - 1;
                        elevator[elevatorid][e_MoveDelay] = GetTickCount() + 2000;

                        elevator[elevatorid][e_QueueFloors][0] = 0;
                        RemoveFirstQueueFloor(elevatorid);
                    }
                    else if(elevator[elevatorid][e_Timer]) {
                        KillTimer(elevator[elevatorid][e_Timer]);
                        elevator[elevatorid][e_Timer] = 0;
                    }
                }
            }
            else {
                elevator[elevatorid][e_DoorStatus] = ELEVATOR_DOOR_OPEN;
            }

            break;
        }
    }

    #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnObjectMoved_elevator
        return OnObjectMoved_elevator(objectid);
    #else 
        return 1;
    #endif
}

#if !defined _inc_y_hooks && !defined _INC_y_hooks
    #if defined _ALS_OnObjectMoved
        #undef OnObjectMoved
    #else
        #define _ALS_OnObjectMoved
    #endif
    #define OnObjectMoved OnObjectMoved_elevator
    #if defined OnObjectMoved_elevator
        forward OnObjectMoved_elevator(objectid);
    #endif 
#endif



forward on_elevator_timer(elevatorid);
public on_elevator_timer(elevatorid) {

    if(!elevator[elevatorid][e_Created]) {
        return 0;
    }

    #if !defined ELEVATOR_DISABLE_DISPLAY
        new Float:X, Float:Y, Float:Z;
       
        if(elevator[elevatorid][e_NextFloor] != -1) {

            GetObjectPos(elevator[elevatorid][e_CabinObject], X, Y, Z);
           
            for(new floor; floor < elevator[elevatorid][e_MaxFloor]; floor ++) {   

                if(Z <= elevatorFloors[elevatorid][floor][ef_DoorZ] - 1.5 || elevatorFloors[elevatorid][floor][ef_DoorZ] + 2.0 <= Z) {
                    continue;
                }

                if(elevator[elevatorid][e_LastFloor] == floor) {
                    break;
                }

                elevator[elevatorid][e_LastFloor] = floor;

                // FIXED: Text does not update after 16 calls against the same object.
                elevator[elevatorid][e_DisplayUpdates] ++;

                if(elevator[elevatorid][e_DisplayUpdates] >= 6) { 
                    E_CreateCabinDisplay(elevatorid);
                }

                SetObjectMaterialText(elevator[elevatorid][e_CabinDisplayObject], 
                    elevatorDisplayText[elevatorid][floor], 
                    0, 80, 
                    "Arial Black", 60, 1, 0xFFFFFFFF, 0x00000000, 1);

                break;
            }
        }
    #endif

    if(elevator[elevatorid][e_MoveDelay] != 0 && elevator[elevatorid][e_MoveDelay] <= GetTickCount()) {
        elevator[elevatorid][e_MoveDelay] = 0;

        if(elevator[elevatorid][e_TargetFloor] == elevator[elevatorid][e_NextFloor]) {
            OpenElevatorDoors(elevatorid, elevator[elevatorid][e_NextFloor]);

            E_CreateCabinText3D(elevatorid);
            elevator[elevatorid][e_NextFloor] = -1;
        }
        else {
            ElevatorMoveToFloor(elevatorid, elevator[elevatorid][e_NextFloor]);

            #if defined OnElevatorMoveToFloor
                OnElevatorMoveToFloor(elevator[elevatorid][e_Name], elevator[elevatorid][e_TargetFloor], elevator[elevatorid][e_NextFloor]);
            #endif
        }
    }
    else if(elevator[elevatorid][e_DoorStatus] == ELEVATOR_DOOR_OPEN && elevator[elevatorid][e_ClosingTime] != 0 && elevator[elevatorid][e_ClosingTime] <= GetTickCount()) {
        elevator[elevatorid][e_ClosingTime] = 0;
        ClosedElevatorDoors(elevatorid, elevator[elevatorid][e_TargetFloor]);
    }

    return 1;
}


// FIXED: Text does not update after 16 calls against the same object.
static stock E_CreateCabinDisplay(elevatorid) {

    elevator[elevatorid][e_DisplayUpdates] = 0;

    new Float:X, Float:Y, Float:Z;
    GetObjectPos(elevator[elevatorid][e_CabinObject], X, Y, Z);

    if(IsValidObject(elevator[elevatorid][e_CabinDisplayObject])) {
        DestroyObject(elevator[elevatorid][e_CabinDisplayObject]);
    }

    elevator[elevatorid][e_CabinDisplayObject] = CreateObject(19476, X, Y, Z, 0.0, 0.0, 0.0, 10.0);
    AttachObjectToObject(elevator[elevatorid][e_CabinDisplayObject], elevator[elevatorid][e_CabinObject], -1.75, 0.0, 1.175, 0.0, 0.0, 0.0);
    return 1;
}


static stock E_ApplyOffset(Float:x, Float:y, Float:z, Float:ang, &Float:fx, &Float:fy, &Float:fz, Float:xOffset = 0.0, Float:yOffset = 0.0, Float:zOffset = 0.0) {

	ang = 360 - ang;

	fx = (floatsin(ang, degrees) * yOffset + floatcos(ang, degrees) * xOffset + x);
	fy = (floatcos(ang, degrees) * yOffset - floatsin(ang, degrees) * xOffset + y);

	fz = z + zOffset;
}


static stock E_CreateCabinText3D(elevatorid) {

    if(IsValidDynamic3DTextLabel(elevator[elevatorid][e_CabinText3D])) {
        DestroyDynamic3DTextLabel(elevator[elevatorid][e_CabinText3D]);
        elevator[elevatorid][e_CabinText3D] = Text3D:INVALID_3DTEXT_ID;
    }

    elevator[elevatorid][e_CabinText3D] = CreateDynamic3DTextLabel(ELEVATOR_TEXT_CABIN, -1, 
        elevator[elevatorid][e_CabinText3D_X], 
        elevator[elevatorid][e_CabinText3D_Y], 
        elevator[elevatorid][e_CabinText3D_Z] - 0.5, 
        10.0, 
        INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 
        1, 
        elevator[elevatorid][e_World], elevator[elevatorid][e_Interior], -1, 10.0);

    return 1;
}