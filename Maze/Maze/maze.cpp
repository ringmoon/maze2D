
#include "Angel.h"
#include <ctime>

//edge class
class edge{
public:
	vec2 points[2];		//a edge has two points
	void setEdge(vec2 point1, vec2 point2){
		points[0] = point1;
		points[1] = point2;
	}
	void through(){
		points[0] = points[1] = vec2(0,0);
	}
	vec2 getPoint1(){
		return points[0];
	}
	vec2 getPoint2(){
		return points[1];
	}
};
//grid class
class grid{
public:			//a grid has four edges
	edge up;	//up edae
	edge down;	//down edge
	edge right;	//right edge
	edge left;	//left edge
	bool isVisited;	//whether be visited
	grid(){}
	void setGrid(vec2 a, vec2 b, vec2 c, vec2 d){
		up.setEdge(a, b);
		right.setEdge(b, c);
		down.setEdge(c, d);
		left.setEdge(d, a);
		isVisited = false;
	}
	//get points of edges
	void getEdgesPoints(vec2 points[],int index){
		points[index]  = up.getPoint1();
		points[index + 1] = up.getPoint2();
		points[index + 2] = right.getPoint1();
		points[index + 3] = right.getPoint2();
		points[index + 4] = down.getPoint1();
		points[index + 5] = down.getPoint2();
		points[index + 6] = left.getPoint1();
		points[index + 7] = left.getPoint2();		
	}
};

const int ROW = 20;  
const int COL = 20;
int firstVisitGridIndex = rand() % (ROW*COL);	//Create a visit start grid
grid grids[ROW*COL];	//create ROW*COL girds
//----------------------------------------------------------------------------
//visit method
void visitGrid(int visitingGridIndex,int fromDirection){
	//case: 0->up 1->right 2->down 3->left 
	// -1->first go in function
	grids[visitingGridIndex].isVisited = true;
	int willGoDirection;
	int nextVisitGridIndex;
	int willVisitDirections[4];
	int visitDirections;

	//find direction of last visit gird
	if (fromDirection == -1){
		willVisitDirections[3] = rand() % 4;
		visitDirections = 4;
	}
	else{
		willVisitDirections[3] = fromDirection;
		visitDirections = 3;
	}
	
	//create will visit order to willVisitDirection array
	for (int i = 0; i < 3; i++){
		int index = rand() % 4;
		willVisitDirections[i] = index;
		if (willVisitDirections[i] == willVisitDirections[3]) {
			i--;
			continue;
		}
		for (int j = 0; j < i; j++){
			if (willVisitDirections[i] == willVisitDirections[j]){
				i--;
				break;
			}
		}	
	}

	for (int i = 0; i < visitDirections; i++){
		switch (willVisitDirections[i])
		{
		case 0:
			if (((visitingGridIndex - COL) >= 0 && !grids[visitingGridIndex - COL].isVisited)){
				nextVisitGridIndex = visitingGridIndex - COL;
				grids[visitingGridIndex].up.through();
				grids[nextVisitGridIndex].down.through();
				willGoDirection = 0;			
				visitGrid(nextVisitGridIndex, (willGoDirection + 2) % 4);
			}
			break;
		case 1:
			if ((((visitingGridIndex + 1) % COL) != 0 && (visitingGridIndex + 1) <ROW*COL&& !grids[visitingGridIndex + 1].isVisited)){
				nextVisitGridIndex = visitingGridIndex + 1;
				grids[visitingGridIndex].right.through();
				grids[nextVisitGridIndex].left.through();
				willGoDirection = 1;
				visitGrid(nextVisitGridIndex, (willGoDirection + 2) % 4);
			}
			break;
		case 2:
			if (((visitingGridIndex + COL) < ROW*COL && !grids[visitingGridIndex + COL].isVisited)){
				nextVisitGridIndex = visitingGridIndex + COL;
				grids[visitingGridIndex].down.through();
				grids[nextVisitGridIndex].up.through();
				willGoDirection = 2;
				visitGrid(nextVisitGridIndex, (willGoDirection + 2) % 4);
			}
			break;
		case 3:
			if ((((visitingGridIndex - 1) % COL) != (COL - 1) && (visitingGridIndex - 1) >= 0 && !grids[visitingGridIndex - 1].isVisited)){
				nextVisitGridIndex = visitingGridIndex - 1;
				grids[visitingGridIndex].left.through();
				grids[nextVisitGridIndex].right.through();
				willGoDirection = 3;
				visitGrid(nextVisitGridIndex, (willGoDirection + 2) % 4);
			}
			break;
		default:
			break;
		}		
	}	
}
//----------------------------------------------------------------------------

void
init( void )
{
	//set ROW*COL grids 
	vec2 points[ROW*COL*8];
	int index = 0;
	float space;
	if (ROW >= COL){
		space = (1.0 - 0.2) / (COL / 2);
	}
	else{
		space = (1.0 - 0.2) / (COL / 2);
	}
	
	vec2 startA = vec2((COL / 2)*(-space), (ROW / 2)*space);
	vec2 startD = vec2((COL / 2)*(-space), (ROW / 2)*(space)-space);
	for (int i = 0; i < ROW; i++){
		for (int j = 0; j < COL; j++){
			grids[i*COL + j].setGrid(startA + j*vec2(space, 0), startA + (j + 1)*vec2(space, 0), startD + (j + 1)*vec2(space, 0), j*vec2(space, 0) + startD);
		}
		startA = startD;
		startD += vec2(0, -space);
	}

	visitGrid(firstVisitGridIndex,-1);

	//clear repeat walls
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (i >= 0 && i<(COL - 1)) {
				grids[i*COL + j].down.through();
			}
			if (j > 0) {
				grids[i*COL + j].left.through();
			}
		}
	}

	//get every points of edges
	for (int i = 0; i < ROW; i++){
		for (int j = 0; j < COL; j++){
			grids[i*COL + j].getEdgesPoints(points, index);
			index += 8;
		}
	}
	//set random exit
	int randomExit = rand() % 4 + 1;
	switch (randomExit){
	case 1:
		points[6] = points[7] = vec2(0, 0);
		break; 
	case 2:
		points[2 + (COL - 1) * 8] = points[3 + (COL - 1) * 8] = vec2(0, 0);
		break;
	case 3:
		points[6 + (ROW - 1)*COL * 8] = points[7 + (ROW - 1)*COL * 8] = vec2(0, 0);
		break;
	case 4:
		points[2 + (COL - 1) * 8 + (ROW - 1)*COL * 8] = points[3 + (COL - 1) * 8 + (ROW - 1)*COL * 8] = vec2(0, 0);
		break;
	}
	
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    glClearColor(0.0,0.0,0.0,1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	glDrawArrays(GL_LINES, 0, ROW*COL*8);    // draw the Lines
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------
int
main( int argc, char **argv )
{
	srand(time(NULL));
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
     glutInitContextVersion( 3, 1 );
     glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Maze2D Game " );

    glewInit();
    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

	glutMainLoop();
	system("pause");
    return 0;
	
}
