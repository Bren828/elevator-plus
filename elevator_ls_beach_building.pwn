/*
*
*
*
*   ---------------------------------------------------
*
*	Creater - @Bren828 					
*									29 June 2023
*
*   Version: 1.0
*
*   ---------------------------------------------------
*
*
*	https://github.com/Bren828/elevator-plus
*
*
*
*/


public OnGameModeInit()
{
    // Create an elevator
    CreateElevator("ls_beach_building", 287.942504, -1609.334838, 33.827411, 0.0, 0.0, 80.0);

    // Create a floor
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,17.8593);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,32.9621);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,47.0014);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,51.5025);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,56.0036);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,60.5047);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,65.0058);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,69.5069);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,73.9923);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,78.4934);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,82.9945);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,87.4956);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,91.9967);
    CreateElevatorFloor("ls_beach_building", 284.8362,-1611.4973,96.4978);

    // Move the elevator to a floor
	SetElevatorFloor("ls_beach_building", 1);
    
    return 1;
}