#define VERTEX(x,y,z,nx,ny,nz,u,v) Vertex { { x,y,z }, { nx,ny,nz }, { u,v } }

VERTEX( -1.0f, -1.0f, 0.0f ,  0,0,1 ,  0,0 ),
VERTEX(  1.0f, -1.0f, 0.0f ,  0,0,1 ,  1,0 ),
VERTEX( -1.0f,  1.0f, 0.0f ,  0,0,1 ,  0,1 ),
VERTEX(  1.0f,  1.0f, 0.0f ,  0,0,1 ,  1,1 ),
VERTEX(  1.0f, -1.0f, 0.0f ,  0,0,1 ,  1,0 ),
VERTEX( -1.0f,  1.0f, 0.0f ,  0,0,1 ,  0,1 ),
