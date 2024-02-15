/*
*
*
*
*   ---------------------------------------------------
*
*	Creater - @Bren828 					
*									15 February 2024
*
*   Version: 1.1
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


#if !defined ELEVATOR_MAX_ELEVATOR
    #define ELEVATOR_MAX_ELEVATOR                   10
#endif

#if !defined ELEVATOR_MAX_FLOORS
    #define ELEVATOR_MAX_FLOORS                     30
#endif

#if !defined ELEVATOR_MAX_NAME_LENGTH
    #define ELEVATOR_MAX_NAME_LENGTH                32
#endif

#if !defined ELEVATOR_DOORS_SPEED
    #define ELEVATOR_DOORS_SPEED                    3.0 // movement speed of the doors
#endif

#if !defined ELEVATOR_SPEED
    #define ELEVATOR_SPEED                          3.0 // movement speed of the elevator
#endif

#if !defined ELEVATOR_UPDATE_TIMER
    #define ELEVATOR_UPDATE_TIMER                   900 
#endif

#if !defined ELEVATOR_DOOR_CLOSING_TIME
    #define ELEVATOR_DOOR_CLOSING_TIME              8000 // milliseconds
#endif

#if !defined ELEVATOR_OBJECT_DISTANCE
    #define ELEVATOR_OBJECT_DISTANCE                100.0
#endif

#if !defined ELEVATOR_ENABLE_DIGITAL_DISPLAY
    #define ELEVATOR_ENABLE_DIGITAL_DISPLAY
#endif

#if !defined ELEVATOR_DIALOG_ID
    #define ELEVATOR_DIALOG_ID                      27303
#endif


#if !defined ELEVATOR_CABIN_TEXT
    static ELEVATOR_CABIN_TEXT[] = "{CCCCCC}~k~~SNEAK_ABOUT~";
#endif

#if !defined ELEVATOR_FLOOR_TEXT
    static ELEVATOR_FLOOR_TEXT[] = "Call the elevator\n{CCCCCC}~k~~SNEAK_ABOUT~{FFFFFF}";
#endif

#if !defined ELEVATOR_FLOOR_IN_DIALOG_TEXT
    static ELEVATOR_FLOOR_IN_DIALOG_TEXT[] = "Floor %d";
#endif

#if !defined ELEVATOR_TEXT_COMING_TO_YOU
    static ELEVATOR_TEXT_COMING_TO_YOU[] =  "{AFAFAF}Already coming to you";
#endif

#if !defined ELEVATOR_TEXT_ALREADY_IN_QUEUE
    static ELEVATOR_TEXT_ALREADY_IN_QUEUE[] = "{AFAFAF}The floor is already in the queue";
#endif

#if !defined ELEVATOR_TEXT_CALLED
    static ELEVATOR_TEXT_CALLED[] = "Elevator called";
#endif

#if !defined ELEVATOR_FLOOR_COLOR_NOW
    static ELEVATOR_FLOOR_COLOR_NOW[] = "{65cd38}";
#endif


enum ELEVATOR_EN
{
    bool:eCreate,
    bool:eElevatorName[ELEVATOR_MAX_NAME_LENGTH],

    eCabinObject,
    eCabinDigitalDisplayObject,
    
    eDoorLObject,
    Float:eDoorLClosed_X,
    Float:eDoorLClosed_Y,
    Float:eDoorLOpen_X,
    Float:eDoorLOpen_Y,
    Float:eDoor_rx, 
    Float:eDoor_ry, 
    Float:eDoor_rz,

    eDoorRObject,
    Float:eDoorRClosed_X,
    Float:eDoorRClosed_Y,
    Float:eDoorROpen_X,
    Float:eDoorROpen_Y,


    Text3D:eText3DCabin,
    Float:Text3DCabin_x,
    Float:Text3DCabin_y,
    Float:Text3DCabin_z,

    eDoorStatus,


    Float:eFloor_X[ELEVATOR_MAX_FLOORS],
    Float:eFloor_Y[ELEVATOR_MAX_FLOORS],
    Float:eFloor_Z[ELEVATOR_MAX_FLOORS],
    Text3D:eFloorText3D[ELEVATOR_MAX_FLOORS],

    eFloorDoorLObject[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorLClosed_X[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorLClosed_Y[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorLOpen_X[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorLOpen_Y[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorHeight[ELEVATOR_MAX_FLOORS],

    eFloorDoorRObject[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorRClosed_X[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorRClosed_Y[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorROpen_X[ELEVATOR_MAX_FLOORS],
    Float:eFloorDoorROpen_Y[ELEVATOR_MAX_FLOORS],

    eMaxFloor,
    eFloor,
    eClosingTime,
    eNextFloor,
    eCurrentFloor,
    eMoveDelay,
    eTotalObjectUpdatesFloor,

    eFloorQueue[ELEVATOR_MAX_FLOORS],
    eTimer
}


static elevator[ELEVATOR_MAX_ELEVATOR][ELEVATOR_EN];

static elevator_text_dialog[ELEVATOR_MAX_ELEVATOR][ELEVATOR_MAX_FLOORS][ELEVATOR_MAX_NAME_LENGTH];

static elevator_text_digital_display[ELEVATOR_MAX_ELEVATOR][ELEVATOR_MAX_FLOORS][ELEVATOR_MAX_NAME_LENGTH];

static max_elevator;

static player_elevator_id[MAX_PLAYERS];


enum (<<=1)
{
    ELEVATOR_DOOR_CLOSED = 0,
    ELEVATOR_DOOR_OPEN = 1,
    ELEVATOR_DOOR_CLOSING = 2,
    ELEVATOR_DOOR_OPENS = 3
}


stock CreateElevator(const elevator_name[], Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if(max_elevator >= ELEVATOR_MAX_ELEVATOR) 
        return print("elevator --->>> | CreateElevator | not all elevators were created! | increase the value '#define ELEVATOR_MAX_ELEVATOR'") & 0;


    elevator[max_elevator][eCreate] = true;
    format(elevator[max_elevator][eElevatorName], ELEVATOR_MAX_NAME_LENGTH, elevator_name);

    elevator[max_elevator][eCabinObject] = CreateObject(18755, x, y, z-0.06, rx, ry, rz, ELEVATOR_OBJECT_DISTANCE);


    #if defined ELEVATOR_ENABLE_DIGITAL_DISPLAY 
        SetObjectMaterial(elevator[max_elevator][eCabinObject], 4, -1, "none", "none", 0xFF000000);

        elevator[max_elevator][eCabinDigitalDisplayObject] = CreateObject(19476, x, y, z, 0.0, 0.0, 0.0, 10.0);
        AttachObjectToObject(elevator[max_elevator][eCabinDigitalDisplayObject], elevator[max_elevator][eCabinObject], -1.75, 0.0, 1.175, 0.0, 0.0, 0.0);
    #endif
  

    new Float:xoff,
        Float:yoff,
        Float:zoff,
        Float:door_z,
        Float:r_rz ;

    //DoorL
    xoff = 0.0, 
    yoff = 0.01, //1.6 - open
    zoff = 0.0;
    r_rz = 360 - rz;
    elevator[max_elevator][eDoorLClosed_X] = floatsin(r_rz, degrees) * yoff + floatcos(r_rz, degrees) * xoff + x;
    elevator[max_elevator][eDoorLClosed_Y] = floatcos(r_rz, degrees) * yoff - floatsin(r_rz, degrees) * xoff + y;
    door_z = zoff + z;

    elevator[max_elevator][eDoor_rx] = rx;
    elevator[max_elevator][eDoor_ry] = ry;
    elevator[max_elevator][eDoor_rz] = rz;

    elevator[max_elevator][eDoorLObject] = CreateObject(18757, elevator[max_elevator][eDoorLClosed_X], elevator[max_elevator][eDoorLClosed_Y], door_z, rx, ry, rz, ELEVATOR_OBJECT_DISTANCE);

    //DoorL open
    yoff = 1.6;
    elevator[max_elevator][eDoorLOpen_X] = floatsin(r_rz, degrees) * yoff + floatcos(r_rz, degrees) * xoff + x;
    elevator[max_elevator][eDoorLOpen_Y] = floatcos(r_rz, degrees) * yoff - floatsin(r_rz, degrees) * xoff + y;



    //DoorR
    xoff = 0.0, 
    yoff = 0.01, //-1.6 - open
    zoff = 0.0;
    elevator[max_elevator][eDoorRClosed_X] = floatsin(r_rz, degrees) * yoff + floatcos(r_rz, degrees) * xoff + x;
    elevator[max_elevator][eDoorRClosed_Y] = floatcos(r_rz, degrees) * yoff - floatsin(r_rz, degrees) * xoff + y;
    door_z = zoff + z;
    elevator[max_elevator][eDoorRObject] = CreateObject(18756, elevator[max_elevator][eDoorRClosed_X], elevator[max_elevator][eDoorRClosed_Y], door_z, rx, ry, rz, ELEVATOR_OBJECT_DISTANCE);

    //DoorR open
    yoff = -1.6;
    elevator[max_elevator][eDoorROpen_X] = floatsin(r_rz, degrees) * yoff + floatcos(r_rz, degrees) * xoff + x;
    elevator[max_elevator][eDoorROpen_Y] = floatcos(r_rz, degrees) * yoff - floatsin(r_rz, degrees) * xoff + y;



    //3DText
    xoff = -1.6, 
    yoff = -1.85, 
    zoff = -0.5;
    elevator[max_elevator][Text3DCabin_x] = floatsin(r_rz, degrees) * yoff + floatcos(r_rz, degrees) * xoff + x;
    elevator[max_elevator][Text3DCabin_y] = floatcos(r_rz, degrees) * yoff - floatsin(r_rz, degrees) * xoff + y;
    elevator[max_elevator][Text3DCabin_z] = zoff + z;

    elevator[max_elevator][eText3DCabin] = Text3D:INVALID_3DTEXT_ID;
    elevator[max_elevator][eDoorStatus] = ELEVATOR_DOOR_CLOSED;
    elevator[max_elevator][eMaxFloor] = 0;
    elevator[max_elevator][eFloor] = 0;
    elevator[max_elevator][eCurrentFloor] = 0;
    elevator[max_elevator][eClosingTime] = 0;
    elevator[max_elevator][eNextFloor] = -1;
    elevator[max_elevator][eTimer] = 0;

    max_elevator ++;
    return 1;
}



stock CreateElevatorFloor(const elevator_name[], Float:x, Float:y, Float:z, Float:door_height = 0.0, const text_dialog[] = "", const text_digital_display[] = "")
{
    new slot = -1,
        max_floor,
        Float:xoff,
        Float:yoff,
        Float:rz;

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++)
    {
        if(elevator[elevatorid][eCreate] == false) 
            continue;

        if(strcmp(elevator_name, elevator[elevatorid][eElevatorName], false)) 
            continue;

        slot = elevatorid;
        max_floor = elevator[elevatorid][eMaxFloor];

        //3DText
        elevator[elevatorid][eFloor_X][max_floor] = x;
        elevator[elevatorid][eFloor_Y][max_floor] = y;
        elevator[elevatorid][eFloor_Z][max_floor] = z;


        if(strlen(text_dialog) >= 1) format(elevator_text_dialog[elevatorid][max_floor], ELEVATOR_MAX_NAME_LENGTH, text_dialog);
        else format(elevator_text_dialog[elevatorid][max_floor], ELEVATOR_MAX_NAME_LENGTH, ELEVATOR_FLOOR_IN_DIALOG_TEXT, max_floor+1);

        if(strlen(text_digital_display) >= 1) format(elevator_text_digital_display[elevatorid][max_floor], ELEVATOR_MAX_NAME_LENGTH, text_digital_display);
        else format(elevator_text_digital_display[elevatorid][max_floor], ELEVATOR_MAX_NAME_LENGTH, "%d", max_floor+1);

        elevator[elevatorid][eFloorText3D][max_floor] = CreateDynamic3DTextLabel(ELEVATOR_FLOOR_TEXT, -1, x, y, z, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 10.0);
       

        //DoorL Closed
        x = elevator[elevatorid][eDoorLClosed_X];
        y = elevator[elevatorid][eDoorLClosed_Y];
        xoff = -0.29, 
        yoff = -0.01, 
        rz = 360 - elevator[elevatorid][eDoor_rz];

        x = floatsin(rz, degrees) * yoff + floatcos(rz, degrees) * xoff + x;
        y = floatcos(rz, degrees) * yoff - floatsin(rz, degrees) * xoff + y;
        elevator[elevatorid][eFloorDoorLObject][max_floor] = CreateDynamicObject(18757, x, y, z+door_height+0.94, elevator[elevatorid][eDoor_rx], elevator[elevatorid][eDoor_ry], elevator[elevatorid][eDoor_rz], -1, -1, -1, ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);
        
        elevator[elevatorid][eFloorDoorLClosed_X][max_floor] = x;
        elevator[elevatorid][eFloorDoorLClosed_Y][max_floor] = y;
        elevator[elevatorid][eFloorDoorHeight][max_floor] = door_height+0.94;


        //DoorL Open
        x = elevator[elevatorid][eDoorLClosed_X];
        y = elevator[elevatorid][eDoorLClosed_Y];
        xoff = -0.29, 
        yoff = 1.6, 

        x = floatsin(rz, degrees) * yoff + floatcos(rz, degrees) * xoff + x;
        y = floatcos(rz, degrees) * yoff - floatsin(rz, degrees) * xoff + y;
        elevator[elevatorid][eFloorDoorLOpen_X][max_floor] = x;
        elevator[elevatorid][eFloorDoorLOpen_Y][max_floor] = y;



        //DoorL Closed
        x = elevator[elevatorid][eDoorRClosed_X];
        y = elevator[elevatorid][eDoorRClosed_Y];
        xoff = -0.29, 
        yoff = -0.01, 

        x = floatsin(rz, degrees) * yoff + floatcos(rz, degrees) * xoff + x;
        y = floatcos(rz, degrees) * yoff - floatsin(rz, degrees) * xoff + y;
        elevator[elevatorid][eFloorDoorRObject][max_floor] = CreateDynamicObject(18756, x, y, z+door_height+0.94, elevator[elevatorid][eDoor_rx], elevator[elevatorid][eDoor_ry], elevator[elevatorid][eDoor_rz], -1, -1, -1, ELEVATOR_OBJECT_DISTANCE, ELEVATOR_OBJECT_DISTANCE);
        elevator[elevatorid][eFloorDoorRClosed_X][max_floor] = x;
        elevator[elevatorid][eFloorDoorRClosed_Y][max_floor] = y;


        //DoorR Open
        x = elevator[elevatorid][eDoorLClosed_X];
        y = elevator[elevatorid][eDoorLClosed_Y];
        xoff = -0.29, 
        yoff = -1.6, 
        x = floatsin(rz, degrees) * yoff + floatcos(rz, degrees) * xoff + x;
        y = floatcos(rz, degrees) * yoff - floatsin(rz, degrees) * xoff + y;
        elevator[elevatorid][eFloorDoorROpen_X][max_floor] = x;
        elevator[elevatorid][eFloorDoorROpen_Y][max_floor] = y;


        elevator[elevatorid][eMaxFloor] ++;
        break;
    }

    if(max_floor == 0 && slot != -1) // moving the elevator to the first floor
    {
        new Float:X, 
            Float:Y, 
            Float:Z;

        elevator[slot][eFloor] = 0;
        elevator[slot][eCurrentFloor] = 0;
        elevator[slot][Text3DCabin_z] = elevator[slot][eFloor_Z][0]+elevator[slot][eFloorDoorHeight][0];

        GetObjectPos(elevator[slot][eCabinObject], X, Y, Z);
        SetObjectPos(elevator[slot][eCabinObject], X, Y, elevator[slot][Text3DCabin_z]-0.06);

        GetObjectPos(elevator[slot][eDoorLObject], X, Y, Z);
        SetObjectPos(elevator[slot][eDoorLObject], X, Y, elevator[slot][Text3DCabin_z]);

        GetObjectPos(elevator[slot][eDoorRObject], X, Y, Z);
        SetObjectPos(elevator[slot][eDoorRObject], X, Y, elevator[slot][Text3DCabin_z]);
        
        elevator[slot][eText3DCabin] = CreateDynamic3DTextLabel(ELEVATOR_CABIN_TEXT, -1, elevator[slot][Text3DCabin_x], elevator[slot][Text3DCabin_y], elevator[slot][Text3DCabin_z]-0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 10.0);

        #if defined ELEVATOR_ENABLE_DIGITAL_DISPLAY
            SetObjectMaterialText(elevator[slot][eCabinDigitalDisplayObject], elevator_text_digital_display[slot][0], 0, 80, "Arial Black", 60, 1, 0xFFFFFFFF, 0x00000000, 1);
        #endif
    }
    return 1;
}


stock SetElevatorFloor(const elevator_name[], floor)
{
    if(max_elevator == 0 || floor < 0 || floor >= ELEVATOR_MAX_FLOORS) 
        return 0;

    new Float:X, 
        Float:Y, 
        Float:Z;

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++)
    {
        if(elevator[elevatorid][eCreate] == false) 
            continue;

        if(strcmp(elevator_name, elevator[elevatorid][eElevatorName], false)) 
            continue;

        if(elevator[elevatorid][eMaxFloor] == 0 || floor >= elevator[elevatorid][eMaxFloor] || floor == elevator[elevatorid][eFloor]) 
            break;
        

        elevator[elevatorid][eFloor] = floor;
        elevator[elevatorid][eCurrentFloor] = floor;
        elevator[elevatorid][Text3DCabin_z] = elevator[elevatorid][eFloor_Z][floor]+elevator[elevatorid][eFloorDoorHeight][floor];


        GetObjectPos(elevator[elevatorid][eCabinObject], X, Y, Z);
        SetObjectPos(elevator[elevatorid][eCabinObject], X, Y, elevator[elevatorid][Text3DCabin_z]-0.06);

        GetObjectPos(elevator[elevatorid][eDoorLObject], X, Y, Z);
        SetObjectPos(elevator[elevatorid][eDoorLObject], X, Y, elevator[elevatorid][Text3DCabin_z]);

        GetObjectPos(elevator[elevatorid][eDoorRObject], X, Y, Z);
        SetObjectPos(elevator[elevatorid][eDoorRObject], X, Y, elevator[elevatorid][Text3DCabin_z]);
        

        DestroyDynamic3DTextLabel(elevator[elevatorid][eText3DCabin]);
        elevator[elevatorid][eText3DCabin] = CreateDynamic3DTextLabel(ELEVATOR_CABIN_TEXT, -1, elevator[elevatorid][Text3DCabin_x], elevator[elevatorid][Text3DCabin_y], elevator[elevatorid][Text3DCabin_z]-0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 10.0);
        

        #if defined ELEVATOR_ENABLE_DIGITAL_DISPLAY

            elevator[elevatorid][eTotalObjectUpdatesFloor] ++; 

            if(elevator[elevatorid][eTotalObjectUpdatesFloor] >= 6) // FIXED: Text does not update after 16 calls against the same object.
            {
                GetObjectPos(elevator[elevatorid][eCabinObject], X, Y, Z);
                DestroyObject(elevator[elevatorid][eCabinDigitalDisplayObject]);

                elevator[elevatorid][eCabinDigitalDisplayObject] = CreateObject(19476, X, Y, Z, 0.0, 0.0, 0.0, 10.0);
                AttachObjectToObject(elevator[elevatorid][eCabinDigitalDisplayObject], elevator[elevatorid][eCabinObject], -1.75, 0.0, 1.175, 0.0, 0.0, 0.0);
            }

            SetObjectMaterialText(elevator[elevatorid][eCabinDigitalDisplayObject], elevator_text_digital_display[elevatorid][floor], 0, 80, "Arial Black", 60, 1, 0xFFFFFFFF, 0x00000000, 1);    
        
        #endif
        return 1;
    }
    return 0;
}


stock GetElevatorObjectID(const elevator_name[], &cabin_id, &doorL_id, &doorR_id, &digital_display_id)
{
    if(max_elevator == 0) 
        return 0;

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++)
    {
        if(elevator[elevatorid][eCreate] == false) 
            continue;

        if(strcmp(elevator_name, elevator[elevatorid][eElevatorName], false)) 
            continue;

        cabin_id = elevator[elevatorid][eCabinObject];
        digital_display_id = elevator[elevatorid][eCabinDigitalDisplayObject];

        doorL_id = elevator[elevatorid][eDoorLObject];
        doorR_id = elevator[elevatorid][eDoorRObject];

        return 1;
    }
    return 0;
}


stock GetElevatorFloorDoorObjectID(const elevator_name[], floor, &doorL_id, &doorR_id)
{
    if(max_elevator == 0 || floor < 0 || floor >= ELEVATOR_MAX_FLOORS) 
        return 0;

    for(new elevatorid; elevatorid < max_elevator; elevatorid ++)
    {
        if(elevator[elevatorid][eCreate] == false) 
            continue;

        if(strcmp(elevator_name, elevator[elevatorid][eElevatorName], false)) 
            continue;

        if(floor >= elevator[elevatorid][eMaxFloor]) 
            return 1;

        doorL_id = elevator[elevatorid][eFloorDoorLObject][floor];
        doorR_id = elevator[elevatorid][eFloorDoorRObject][floor];

        return 1;
    }
    return 0;
}


static stock OpenElevatorDoors(elevatorid, floorid)
{
    new Float:X, 
        Float:Y, 
        Float:Z;

    GetObjectPos(elevator[elevatorid][eDoorLObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorLObject], elevator[elevatorid][eDoorLOpen_X], elevator[elevatorid][eDoorLOpen_Y], Z, ELEVATOR_DOORS_SPEED);

    GetObjectPos(elevator[elevatorid][eDoorRObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorRObject], elevator[elevatorid][eDoorROpen_X], elevator[elevatorid][eDoorROpen_Y], Z, ELEVATOR_DOORS_SPEED);
    
    
    GetDynamicObjectPos(elevator[elevatorid][eFloorDoorLObject][floorid], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][eFloorDoorLObject][floorid], elevator[elevatorid][eFloorDoorLOpen_X][floorid], elevator[elevatorid][eFloorDoorLOpen_Y][floorid], Z, ELEVATOR_DOORS_SPEED);

    GetDynamicObjectPos(elevator[elevatorid][eFloorDoorRObject][floorid], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][eFloorDoorRObject][floorid], elevator[elevatorid][eFloorDoorROpen_X][floorid], elevator[elevatorid][eFloorDoorROpen_Y][floorid], Z, ELEVATOR_DOORS_SPEED);


    elevator[elevatorid][eClosingTime] = GetTickCount() + ELEVATOR_DOOR_CLOSING_TIME;
    elevator[elevatorid][eDoorStatus] = ELEVATOR_DOOR_OPENS;
    return 1;
}


static stock ClosedElevatorDoors(elevatorid, floorid)
{
    if(elevator[elevatorid][eDoorStatus] == ELEVATOR_DOOR_CLOSED) 
    {
        elevator[elevatorid][eMoveDelay] = GetTickCount() + 2000;
        return 1;
    }

    new Float:X, 
        Float:Y, 
        Float:Z;

    GetObjectPos(elevator[elevatorid][eDoorLObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorLObject], elevator[elevatorid][eDoorLClosed_X], elevator[elevatorid][eDoorLClosed_Y], Z, ELEVATOR_DOORS_SPEED);

    GetObjectPos(elevator[elevatorid][eDoorRObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorRObject], elevator[elevatorid][eDoorRClosed_X], elevator[elevatorid][eDoorRClosed_Y], Z, ELEVATOR_DOORS_SPEED);
    
    
    GetDynamicObjectPos(elevator[elevatorid][eFloorDoorLObject][floorid], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][eFloorDoorLObject][floorid], elevator[elevatorid][eFloorDoorLClosed_X][floorid], elevator[elevatorid][eFloorDoorLClosed_Y][floorid], Z, ELEVATOR_DOORS_SPEED);

    GetDynamicObjectPos(elevator[elevatorid][eFloorDoorRObject][floorid], X, Y, Z);
    MoveDynamicObject(elevator[elevatorid][eFloorDoorRObject][floorid], elevator[elevatorid][eFloorDoorRClosed_X][floorid], elevator[elevatorid][eFloorDoorRClosed_Y][floorid], Z, ELEVATOR_DOORS_SPEED);


    elevator[elevatorid][eClosingTime] = 0;
    elevator[elevatorid][eDoorStatus] = ELEVATOR_DOOR_CLOSING;
    return 1;
}


static stock ElevatorMoveToFloor(elevatorid, next_floor)
{
    new Float:X, 
        Float:Y, 
        Float:Z;

    GetObjectPos(elevator[elevatorid][eCabinObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eCabinObject], X, Y, elevator[elevatorid][eFloor_Z][next_floor]+elevator[elevatorid][eFloorDoorHeight][next_floor]-0.06, ELEVATOR_SPEED);
    

    GetObjectPos(elevator[elevatorid][eDoorLObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorLObject], elevator[elevatorid][eDoorLClosed_X], elevator[elevatorid][eDoorLClosed_Y], elevator[elevatorid][eFloor_Z][next_floor]+elevator[elevatorid][eFloorDoorHeight][next_floor], ELEVATOR_SPEED);


    GetObjectPos(elevator[elevatorid][eDoorRObject], X, Y, Z);
    MoveObject(elevator[elevatorid][eDoorRObject], elevator[elevatorid][eDoorRClosed_X], elevator[elevatorid][eDoorRClosed_Y], elevator[elevatorid][eFloor_Z][next_floor]+elevator[elevatorid][eFloorDoorHeight][next_floor], ELEVATOR_SPEED);
    

    DestroyDynamic3DTextLabel(elevator[elevatorid][eText3DCabin]);
    elevator[elevatorid][Text3DCabin_z] = elevator[elevatorid][eFloor_Z][next_floor]+elevator[elevatorid][eFloorDoorHeight][next_floor];
    return 1;
}


static stock IsFloorInQueue(elevatorid, floor)
{
    for(new queue; queue < elevator[elevatorid][eMaxFloor]; queue ++)
    {
        if(elevator[elevatorid][eFloorQueue][queue] == 0) 
            continue;

        if(elevator[elevatorid][eFloorQueue][queue] != floor+1) 
            continue;

        return 1;
    }
    return 0;
}


static stock RemoveFirstQueueFloor(elevatorid)
{
    new max_queue, 
        floor[ELEVATOR_MAX_FLOORS];

    for(new queue; queue < elevator[elevatorid][eMaxFloor]; queue ++)
    {
        if(elevator[elevatorid][eFloorQueue][queue] == 0) 
            continue;

        floor[max_queue] = elevator[elevatorid][eFloorQueue][queue];
        elevator[elevatorid][eFloorQueue][queue] = 0;

        max_queue++;
    }

    for(new x; x < max_queue; x ++) 
    {
        elevator[elevatorid][eFloorQueue][x] = floor[x];
    }
    return 1;
}


#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#else
    public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#endif
{
    if(newkeys & KEY_WALK && max_elevator >= 1)
    {
        new elevatorid = -1,
            type,
            floor;
            
        for(new x; x < max_elevator; x ++)
        {
            if(elevator[x][eCreate] == false) 
                continue;

            if(elevator[x][eMaxFloor] == 0) 
                continue;

            if(IsPlayerInRangeOfPoint(playerid, 1.1, elevator[x][Text3DCabin_x], elevator[x][Text3DCabin_y], elevator[x][Text3DCabin_z]))
            {
                if(elevator[x][eNextFloor] != -1) break; 

                elevatorid = x;
                type = 0;
                break;
            }

            for(new floor_id; floor_id < elevator[x][eMaxFloor]; floor_id ++)
            {
                if(!IsPlayerInRangeOfPoint(playerid, 1.0, elevator[x][eFloor_X][floor_id], elevator[x][eFloor_Y][floor_id], elevator[x][eFloor_Z][floor_id])) 
                    continue;
                
                elevatorid = x;
                floor = floor_id;
                type = 1;
                break;
            }

            if(elevatorid != -1) break;
        }

        if(elevatorid != -1)
        {
            if(type == 0)
            {
                if(elevator[elevatorid][eNextFloor] != -1) 
                    return 1;

                player_elevator_id[playerid] = elevatorid;
                
                new string[(8+9+ELEVATOR_MAX_NAME_LENGTH) * ELEVATOR_MAX_FLOORS],
                    color[9];

                for(new x; x < elevator[elevatorid][eMaxFloor]; x ++)
                {
                    if(elevator[elevatorid][eFloor] == x) color = ELEVATOR_FLOOR_COLOR_NOW;
                    else color = "";

                    format(string, sizeof(string), "%s%s%s\n", string, color, elevator_text_dialog[elevatorid][x]);
                }

                #if defined _mdialog_included
                    Dialog_Open(playerid, Dialog:elevator_floors, DIALOG_STYLE_LIST, "Select floor", string, "Select", "Close");
                #else
                    ShowPlayerDialog(playerid, ELEVATOR_DIALOG_ID, DIALOG_STYLE_LIST, "Select floor", string, "Select", "Close");
                #endif
                return 1;
            }


            if(elevator[elevatorid][eNextFloor] != -1 || elevator[elevatorid][eClosingTime] != 0 && elevator[elevatorid][eFloor] != floor)
            {
                if(elevator[elevatorid][eNextFloor] != floor)
                {
                    if(IsFloorInQueue(elevatorid, floor)) return SendClientMessage(playerid, -1, ELEVATOR_TEXT_ALREADY_IN_QUEUE);

                    for(new queue; queue < elevator[elevatorid][eMaxFloor]; queue ++)
                    {
                        if(elevator[elevatorid][eFloorQueue][queue] != 0) 
                            continue;

                        elevator[elevatorid][eFloorQueue][queue] = floor+1;
                        break;
                    }

                    SendClientMessage(playerid, -1, ELEVATOR_TEXT_CALLED);
                }
                else SendClientMessage(playerid, -1, ELEVATOR_TEXT_COMING_TO_YOU);
                return 1;
            }


            if(elevator[elevatorid][eDoorStatus] != ELEVATOR_DOOR_CLOSED) 
                return 1;

            if(elevator[elevatorid][eTimer] == 0) elevator[elevatorid][eTimer] = SetTimerEx("on_elevator_timer", ELEVATOR_UPDATE_TIMER, true, "d", elevatorid);

            if(elevator[elevatorid][eFloor] == floor) 
            {
                OpenElevatorDoors(elevatorid, floor);
            }
            else 
            {
                elevator[elevatorid][eNextFloor] = floor;
                ClosedElevatorDoors(elevatorid, elevator[elevatorid][eFloor]);
                SendClientMessage(playerid, -1, ELEVATOR_TEXT_CALLED);
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
            if(!response || listitem < 0 || listitem >= ELEVATOR_MAX_FLOORS) 
                return 1;

            new elevatorid = player_elevator_id[playerid];

            if(elevator[elevatorid][eNextFloor] != -1) 
                return 1;

            if(elevator[elevatorid][eTimer] == 0) elevator[elevatorid][eTimer] = SetTimerEx("on_elevator_timer", ELEVATOR_UPDATE_TIMER, true, "d", elevatorid);    

            if(elevator[elevatorid][eFloor] == listitem)
            {
                if(elevator[elevatorid][eDoorStatus] == ELEVATOR_DOOR_CLOSED) OpenElevatorDoors(elevatorid, elevator[elevatorid][eFloor]);
                return 1;
            }

            elevator[elevatorid][eNextFloor] = listitem;
            ClosedElevatorDoors(elevatorid, elevator[elevatorid][eFloor]);
            return 1;

    #if !defined _mdialog_included
        }
        #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnDialogResponse_elevator
            OnDialogResponse_elevator(playerid, dialogid, response, listitem, inputtext);
        #endif
        return 1;
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
    hook OnObjectMoved(objectid)
#else
    public OnObjectMoved(objectid)
#endif
{
    if(max_elevator >= 1)
    {
        for(new elevatorid; elevatorid < max_elevator; elevatorid ++)
        {
            if(elevator[elevatorid][eCreate] == false) 
                continue;

            if(elevator[elevatorid][eDoorLObject] != objectid) 
                continue;

            if(elevator[elevatorid][eDoorStatus] == ELEVATOR_DOOR_CLOSED && elevator[elevatorid][eNextFloor] != -1)
            {
                // the elevator arrived

                elevator[elevatorid][eMoveDelay] = GetTickCount() + 2000;  
                elevator[elevatorid][eFloor] = elevator[elevatorid][eNextFloor];

                if(elevator[elevatorid][eFloorQueue][0] != 0)
                {
                    for(new queue; queue < elevator[elevatorid][eMaxFloor]; queue ++)
                    {
                        if(elevator[elevatorid][eFloorQueue][queue] == 0) 
                            continue;

                        if(elevator[elevatorid][eFloorQueue][queue] != elevator[elevatorid][eFloor]+1) 
                            continue;

                        elevator[elevatorid][eFloorQueue][queue] = 0;
                        RemoveFirstQueueFloor(elevatorid);
                        break;
                    }
                }

                #if defined OnElevatorArrivedFloor
                    OnElevatorArrivedFloor(elevator[elevatorid][eElevatorName], elevator[elevatorid][eFloor]);
                #endif
            }
            else if(elevator[elevatorid][eDoorStatus] == ELEVATOR_DOOR_CLOSING)
            {
                elevator[elevatorid][eDoorStatus] = ELEVATOR_DOOR_CLOSED;
                if(elevator[elevatorid][eNextFloor] != -1) 
                {
                    // We send the elevator

                    elevator[elevatorid][eMoveDelay] = GetTickCount() + 2000;
                }
                else // check queue
                {
                    if(elevator[elevatorid][eFloorQueue][0] != 0)
                    {
                        elevator[elevatorid][eNextFloor] = elevator[elevatorid][eFloorQueue][0]-1;
                        elevator[elevatorid][eMoveDelay] = GetTickCount() + 2000;

                        elevator[elevatorid][eFloorQueue][0] = 0;
                        RemoveFirstQueueFloor(elevatorid);
                    }
                    else if(elevator[elevatorid][eTimer] != 0)
                    {
                        KillTimer(elevator[elevatorid][eTimer]);
                        elevator[elevatorid][eTimer] = 0;
                    }
                }
            }
            else 
            {
                elevator[elevatorid][eDoorStatus] = ELEVATOR_DOOR_OPEN;
            }
            break;
        }
    }

    #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnObjectMoved_elevator
        OnObjectMoved_elevator(objectid);
    #endif

    return 1;
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
public on_elevator_timer(elevatorid)
{
    if(elevator[elevatorid][eCreate] == false) return 1;


    #if defined ELEVATOR_ENABLE_DIGITAL_DISPLAY
        new Float:X, 
            Float:Y, 
            Float:Z;
       
        if(elevator[elevatorid][eNextFloor] != -1)
        {
            GetObjectPos(elevator[elevatorid][eCabinObject], X, Y, Z);

            for(new floor; floor < elevator[elevatorid][eMaxFloor]; floor ++)
            {   
                if(Z <= elevator[elevatorid][eFloor_Z][floor]-1.5 || elevator[elevatorid][eFloor_Z][floor]+2.0 <= Z) 
                    continue;

                if(elevator[elevatorid][eCurrentFloor] == floor) 
                    break;

                elevator[elevatorid][eCurrentFloor] = floor;
                elevator[elevatorid][eTotalObjectUpdatesFloor] ++;

                if(elevator[elevatorid][eTotalObjectUpdatesFloor] >= 6) // FIXED: Text does not update after 16 calls against the same object.
                {
                    DestroyObject(elevator[elevatorid][eCabinDigitalDisplayObject]);
                    elevator[elevatorid][eTotalObjectUpdatesFloor] = 0;
                    
                    elevator[elevatorid][eCabinDigitalDisplayObject] = CreateObject(19476, X, Y, Z, 0.0, 0.0, 0.0, 10.0);
                    AttachObjectToObject(elevator[elevatorid][eCabinDigitalDisplayObject], elevator[elevatorid][eCabinObject], -1.75, 0.0, 1.175, 0.0, 0.0, 0.0);
                }

                SetObjectMaterialText(elevator[elevatorid][eCabinDigitalDisplayObject], elevator_text_digital_display[elevatorid][floor], 0, 80, "Arial Black", 60, 1, 0xFFFFFFFF, 0x00000000, 1); 
                break;
            }
        }
    #endif


    if(elevator[elevatorid][eMoveDelay] != 0 && elevator[elevatorid][eMoveDelay] <= GetTickCount())
    {
        elevator[elevatorid][eMoveDelay] = 0;
        if(elevator[elevatorid][eFloor] == elevator[elevatorid][eNextFloor])
        {
            OpenElevatorDoors(elevatorid, elevator[elevatorid][eNextFloor]);

            elevator[elevatorid][eText3DCabin] = CreateDynamic3DTextLabel(ELEVATOR_CABIN_TEXT, -1, elevator[elevatorid][Text3DCabin_x], elevator[elevatorid][Text3DCabin_y], elevator[elevatorid][Text3DCabin_z]-0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 10.0);
            
            elevator[elevatorid][eNextFloor] = -1;
        }
        else 
        {
            ElevatorMoveToFloor(elevatorid, elevator[elevatorid][eNextFloor]);

            #if defined OnElevatorMoveToFloor
                OnElevatorMoveToFloor(elevator[elevatorid][eElevatorName], elevator[elevatorid][eFloor], elevator[elevatorid][eNextFloor]);
            #endif
        }
    }
    else if(elevator[elevatorid][eDoorStatus] == ELEVATOR_DOOR_OPEN && elevator[elevatorid][eClosingTime] != 0 && elevator[elevatorid][eClosingTime] <= GetTickCount())
    {
        elevator[elevatorid][eClosingTime] = 0;
        ClosedElevatorDoors(elevatorid, elevator[elevatorid][eFloor]);
    }


    return 1;
}


