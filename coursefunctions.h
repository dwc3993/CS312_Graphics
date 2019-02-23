#include "definitions.h"

#ifndef COURSE_FUNCTIONS_H
#define COURSE_FUNCTIONS_H

/***************************************************
 * Team Activity for week #1.
 * When working on this activity be sure to 
 * comment out the following function calls in 
 * pipeline.cpp:main():
 *      1) processUserInputs(running);
 *      2) clearScreen(frame);
 *      3) Any draw calls that are being made there
 * 
 * When you finish this activity be sure to 
 * uncomment these functions again!!!
 **************************************************/
void GameOfLife(Buffer2D<PIXEL> & target)
{
        // 'Static's are initialized exactly once
        static bool isSetup = true;
        static bool holdDown = false;
        static int w = target.width();
        static int h = target.height();
        static int scaleFactor = 8;
        static int gridW = 64;
        static int gridH = 64; 
        static int grid[64][64];
        static int gridTmp[64][64];

        // Setup small grid, temporary grid from previous iteration
        for(int y = 0; y < gridH; y++)
        {
                for(int x = 0; x < gridW; x++)
                {
                        grid[y][x] = (target[y*scaleFactor][x*scaleFactor] == 0xffff0000) ? 1 : 0;
                        gridTmp[y][x] = grid[y][x];
                }
        }

        //Parse for inputs
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
                int mouseX;
                int mouseY;
                if(e.type == SDL_MOUSEBUTTONDOWN)
                {
                        holdDown = true;
                }
                if(e.type == SDL_MOUSEBUTTONUP)
                {
                        holdDown = false;
                }
                if(e.key.keysym.sym == 'g' && e.type == SDL_KEYDOWN) 
                {
                        isSetup = !isSetup;
                }
                if(holdDown && isSetup)
                {
                        // Clicking the mouse changes a pixel's color
                        SDL_GetMouseState(&mouseX, &mouseY);
                        int gridX = mouseX / scaleFactor;
                        int gridY = mouseY / scaleFactor;
                        if(grid[gridY][gridX] == 1)
                        {
                                // Dead
                                grid[gridY][gridX] = 0;
                        }
                        else
                        {
                                // Alive
                                grid[gridY][gridX] = 1;
                        }
                }
        }


        // Advance the simulation after pressing 'g'
        if(!isSetup)
        {
                // Your Code goes here

                // Wait a half-second between iterations
                SDL_Delay(500);
        }


        // Upscale/blit to screen
        for(int y = 0; y < h; y++)
        {
                for(int x = 0; x < w; x++)
                {
                        int yScal = y/scaleFactor;
                        int xScal = x/scaleFactor;
                        if(grid[yScal][xScal] == 0)
                        {
                                // Dead Color
                                target[y][x] = 0xff000000;
                        }
                        else
                        {
                                // Alive color
                                target[y][x] = 0xffff0000;
                        }
                }
        }
}

/***************************************************
 * Create a 3D View like in a CAD program
 * NOTE: Assumes that the resolution is an even 
 * value in both dimensions.
 **************************************************/
void CADView(Buffer2D<PIXEL> & target)
{
        // Each CAD Quadrant
        static int halfWid = target.width()/2;
        static int halfHgt = target.height()/2;
        static Buffer2D<PIXEL> topLeft(halfWid, halfHgt);
        static Buffer2D<PIXEL> topRight(halfWid, halfHgt);
        static Buffer2D<PIXEL> botLeft(halfWid, halfHgt);
        static Buffer2D<PIXEL> botRight(halfWid, halfHgt);


        // Your code goes here 
        // Feel free to copy from other test functions to get started!


        // Blit four panels to target
        int yStartSrc = 0;
        int xStartSrc = 0;
        int yLimitSrc = topLeft.height();
        int xLimitSrc = topLeft.width();
        for(int ySrc = yStartSrc; ySrc < yLimitSrc; ySrc++)
        {
                for(int xSrc = xStartSrc; xSrc < xLimitSrc; xSrc++)
                {
                        target[ySrc][xSrc]                 = botLeft[ySrc][xSrc];
                        target[ySrc][xSrc+halfWid]         = botRight[ySrc][xSrc];
                        target[ySrc+halfHgt][xSrc]         = topLeft[ySrc][xSrc];
                        target[ySrc+halfHgt][xSrc+halfWid] = topRight[ySrc][xSrc];
                }
        }
}

/***************************************************
 * Demonstrate pixel drawing for project 01.
 **************************************************/
void TestDrawPixel(Buffer2D<PIXEL> & target)
{
        Vertex vert = {10, 502, 1, 1};
        Attributes pointAttributes;
        PIXEL color = 0xffff0000;
        pointAttributes.color = color;   

        DrawPrimitive(POINT, target, &vert, &pointAttributes);
}

/***********************************************
 * Demonstrate Triangle Drawing for Project 02. 
 **********************************************/
void TestDrawTriangle(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 6 Flat color triangles below
        *************************************************/
        Vertex verts[3];
        Attributes attr[3];
        verts[0] = {100, 362, 1, 1};
        verts[1] = {150, 452, 1, 1};
        verts[2] = {50, 452, 1, 1};
        PIXEL colors1[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors1[0];
        
        DrawPrimitive(TRIANGLE, target, verts, attr);

        verts[0] = {300, 402, 1, 1};
        verts[1] = {250, 452, 1, 1};
        verts[2] = {250, 362, 1, 1};
        PIXEL colors2[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors2[1];

        DrawPrimitive(TRIANGLE, target, verts, attr);

        verts[0] = {450, 362, 1, 1};
        verts[1] = {450, 452, 1, 1};
        verts[2] = {350, 402, 1, 1};
        PIXEL colors3[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors3[2];

        DrawPrimitive(TRIANGLE, target, verts, attr);
        
        verts[0] = {110, 262, 1, 1};
        verts[1] = {60, 162, 1, 1};
        verts[2] = {150, 162, 1, 1};
        PIXEL colors4[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors4[0];

        DrawPrimitive(TRIANGLE, target, verts, attr);

        verts[0] = {210, 252, 1, 1};
        verts[1] = {260, 172, 1, 1};
        verts[2] = {310, 202, 1, 1};
        PIXEL colors5[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors5[1];

        DrawPrimitive(TRIANGLE, target, verts, attr);
        
        verts[0] = {370, 202, 1, 1};
        verts[1] = {430, 162, 1, 1};
        verts[2] = {470, 252, 1, 1};
        PIXEL colors6[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your color code goes here for 'attr'
        attr[0].color = colors6[2];

        DrawPrimitive(TRIANGLE, target, verts, attr);
}

/***********************************************
 * Linear Interpolate of the values for the FragShader
 * *********************************************/
double interp(double area, double det[], double one, double two, double three)
{
        return ((det[2] * two + det[0] * three + det[1] * one) / area);
}

/***********************************************
 * Demonstrate Fragment Shader, linear VBO 
 * interpolation for Project 03. 
 **********************************************/
void TestDrawFragments(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Interpolated color triangle
        *************************************************/
        Vertex colorTriangle[3];
        Attributes colorAttributes[3];
        colorTriangle[0] = {250, 112, 1, 1};
        colorTriangle[1] = {450, 452, 1, 1};
        colorTriangle[2] = {50, 452, 1, 1};
        PIXEL colors[3] = {0xffff0000, 0xff00ff00, 0xff0000ff}; // Or {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}}

        // store all the color values that we will need
        colorAttributes[0].value.push_back(1.0);
       	colorAttributes[0].value.push_back(0.0);
	colorAttributes[0].value.push_back(0.0);
	colorAttributes[1].value.push_back(0.0);
	colorAttributes[1].value.push_back(1.0);
	colorAttributes[1].value.push_back(0.0);
	colorAttributes[2].value.push_back(0.0);
	colorAttributes[2].value.push_back(0.0);
	colorAttributes[2].value.push_back(1.0);

        // set up the FragShader for color attributes
        FragmentShader myColorFragShader;
	myColorFragShader.FragShader = ColorFragShader;
        // myColorFragShader.FragShader = FragShaderUVwithoutImage;

        Attributes colorUniforms;
	// Nothing gets setup this time

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader);

        /****************************************************
         * 2. Interpolated image triangle
        ****************************************************/
        Vertex imageTriangle[3];
        Attributes imageAttributes[3];
        imageTriangle[0] = {425, 112, 1, 1};
        imageTriangle[1] = {500, 252, 1, 1};
        imageTriangle[2] = {350, 252, 1, 1};
        double coordinates[3][2] = { {1,0}, {1,1}, {0,1} };

        // store the x and y coordinates for the image
        imageAttributes[0].value.push_back(coordinates[0][0]);
        imageAttributes[0].value.push_back(coordinates[0][1]);
        imageAttributes[1].value.push_back(coordinates[1][0]);
        imageAttributes[1].value.push_back(coordinates[1][1]);
        imageAttributes[2].value.push_back(coordinates[2][0]);
        imageAttributes[2].value.push_back(coordinates[2][1]);

        // get the image that will be used
        static BufferImage myImage("checker.bmp");

        Attributes imageUniforms;
        imageUniforms.ptrImgs.push_back(&myImage);

        // set up the FragShader for color attributes
        FragmentShader myImageFragShader;
        myImageFragShader.FragShader = ImageFragShader;

        DrawPrimitive(TRIANGLE, target, imageTriangle, imageAttributes, &imageUniforms, &myImageFragShader);
}

/************************************************
 * Demonstrate Perspective correct interpolation 
 * for Project 04. 
 ***********************************************/
void TestDrawPerspectiveCorrect(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Image quad (2 TRIs) Code (texture interpolated)
        **************************************************/
        // Artificially projected, viewport transformed
        double divA = 6;
        double divB = 40;

        // There could be a Vertex "cube" instead of repeating vertices across multiple quads
        // I'm sacrificing performance for readability here a little bit
        Vertex quad[] = {{(-1200 / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200  / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200  / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                         {(-1200 / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        Vertex backWall[] = {{(-1200 / divB) + (S_WIDTH/2), (1500 / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                          {(1200  / divB) + (S_WIDTH/2), (1500 / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                          {(1200  / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                          {(-1200 / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        Vertex leftWall[] = {{(-1200 / divA) + (S_WIDTH/2), (1500  / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(-1200 / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(-1200 / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                         {(-1200 / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        Vertex rightWall[] = {{(1200  / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200 / divA) + (S_WIDTH/2), (1500  / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200 / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                         {(1200  / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        Vertex top[] = {{(1200 / divA) + (S_WIDTH/2), (1500  / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(-1200 / divA) + (S_WIDTH/2), (1500  / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(-1200 / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                         {(1200  / divB) + (S_WIDTH/2), (4500 / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        // Floor vertices and attributes
        Vertex verticesImgA[3];
        Attributes imageAttributesA[3];
        verticesImgA[0] = quad[0];
        verticesImgA[1] = quad[1];
        verticesImgA[2] = quad[2];

        Vertex verticesImgB[3];        
        Attributes imageAttributesB[3];
        verticesImgB[0] = quad[2];
        verticesImgB[1] = quad[3];
        verticesImgB[2] = quad[0];

        // Back Walls vertices and attributes
        Vertex vertsBackA[3];
        Attributes backWallAttrsA[3];
        vertsBackA[0] = backWall[0];
        vertsBackA[1] = backWall[1];
        vertsBackA[2] = backWall[2];

        Vertex vertsBackB[3];        
        Attributes backWallAttrsB[3];
        vertsBackB[0] = backWall[2];
        vertsBackB[1] = backWall[3];
        vertsBackB[2] = backWall[0];

        // Left Wall vertices and attributes
        Vertex vertsLeftA[3];
        Attributes leftWallAttrsA[3];
        vertsLeftA[0] = leftWall[0];
        vertsLeftA[1] = leftWall[1];
        vertsLeftA[2] = leftWall[2];

        Vertex vertsLeftB[3];        
        Attributes leftWallAttrsB[3];
        vertsLeftB[0] = leftWall[2];
        vertsLeftB[1] = leftWall[3];
        vertsLeftB[2] = leftWall[0];

        // Right Wall vertices and attributes
        Vertex vertsRightA[3];        
        Attributes rightWallAttrsA[3];
        vertsRightA[0] = rightWall[0];
        vertsRightA[1] = rightWall[1];
        vertsRightA[2] = rightWall[2];

        Vertex vertsRightB[3];        
        Attributes rightWallAttrsB[3];
        vertsRightB[0] = rightWall[2];
        vertsRightB[1] = rightWall[3];
        vertsRightB[2] = rightWall[0];

        // Ceiling vertices and attributes
        Vertex vertsTopA[3];        
        Attributes topWallAttrsA[3];
        vertsTopA[0] = top[0];
        vertsTopA[1] = top[1];
        vertsTopA[2] = top[2];

        Vertex vertsTopB[3];        
        Attributes topWallAttrsB[3];
        vertsTopB[0] = top[2];
        vertsTopB[1] = top[3];
        vertsTopB[2] = top[0];

        // This could be one array of coordinates instead of repeating
        // I'm sacrificing performance for readability a little bit here
        double coordinates[4][2] = { {0/divA,0/divA}, {1/divA,0/divA}, {1/divB,1/divB}, {0/divB,1/divB} };
        double coordinatesBack[4][2] = { {0/divB,0/divB}, {1/divB,0/divB}, {1/divB,1/divB}, {0/divB,1/divB} };
        double coordinatesLeft[4][2] = { {0/divA,0/divA}, {1/divA,0/divA}, {1/divB,1/divB}, {0/divB,1/divB} };
        double coordinatesRight[4][2] = { {0/divA,0/divA}, {1/divA,0/divA}, {1/divB,1/divB}, {0/divB,1/divB} };
        double coordinatesTop[4][2] = { {0/divA,0/divA}, {1/divA,0/divA}, {1/divB,1/divB}, {0/divB,1/divB} };

        // Floor Attributes
        imageAttributesA[0].value.push_back(coordinates[0][0]);
        imageAttributesA[0].value.push_back(coordinates[0][1]);
        imageAttributesA[1].value.push_back(coordinates[1][0]);
        imageAttributesA[1].value.push_back(coordinates[1][1]);
        imageAttributesA[2].value.push_back(coordinates[2][0]);
        imageAttributesA[2].value.push_back(coordinates[2][1]);

        imageAttributesB[0].value.push_back(coordinates[2][0]);
        imageAttributesB[0].value.push_back(coordinates[2][1]);
        imageAttributesB[1].value.push_back(coordinates[3][0]);
        imageAttributesB[1].value.push_back(coordinates[3][1]);
        imageAttributesB[2].value.push_back(coordinates[0][0]);
        imageAttributesB[2].value.push_back(coordinates[0][1]);

        // Back Wall Attributes
        backWallAttrsA[0].value.push_back(coordinatesBack[0][0]);
        backWallAttrsA[0].value.push_back(coordinatesBack[0][1]);
        backWallAttrsA[1].value.push_back(coordinatesBack[1][0]);
        backWallAttrsA[1].value.push_back(coordinatesBack[1][1]);
        backWallAttrsA[2].value.push_back(coordinatesBack[2][0]);
        backWallAttrsA[2].value.push_back(coordinatesBack[2][1]);

        backWallAttrsB[0].value.push_back(coordinatesBack[2][0]);
        backWallAttrsB[0].value.push_back(coordinatesBack[2][1]);
        backWallAttrsB[1].value.push_back(coordinatesBack[3][0]);
        backWallAttrsB[1].value.push_back(coordinatesBack[3][1]);
        backWallAttrsB[2].value.push_back(coordinatesBack[0][0]);
        backWallAttrsB[2].value.push_back(coordinatesBack[0][1]);

        // Left Wall Attributes
        leftWallAttrsA[0].value.push_back(coordinatesLeft[0][0]);
        leftWallAttrsA[0].value.push_back(coordinatesLeft[0][1]);
        leftWallAttrsA[1].value.push_back(coordinatesLeft[1][0]);
        leftWallAttrsA[1].value.push_back(coordinatesLeft[1][1]);
        leftWallAttrsA[2].value.push_back(coordinatesLeft[2][0]);
        leftWallAttrsA[2].value.push_back(coordinatesLeft[2][1]);

        leftWallAttrsB[0].value.push_back(coordinatesLeft[2][0]);
        leftWallAttrsB[0].value.push_back(coordinatesLeft[2][1]);
        leftWallAttrsB[1].value.push_back(coordinatesLeft[3][0]);
        leftWallAttrsB[1].value.push_back(coordinatesLeft[3][1]);
        leftWallAttrsB[2].value.push_back(coordinatesLeft[0][0]);
        leftWallAttrsB[2].value.push_back(coordinatesLeft[0][1]);

        // Right Wall Attributes
        rightWallAttrsA[0].value.push_back(coordinatesRight[0][0]);
        rightWallAttrsA[0].value.push_back(coordinatesRight[0][1]);
        rightWallAttrsA[1].value.push_back(coordinatesRight[1][0]);
        rightWallAttrsA[1].value.push_back(coordinatesRight[1][1]);
        rightWallAttrsA[2].value.push_back(coordinatesRight[2][0]);
        rightWallAttrsA[2].value.push_back(coordinatesRight[2][1]);

        rightWallAttrsB[0].value.push_back(coordinatesRight[2][0]);
        rightWallAttrsB[0].value.push_back(coordinatesRight[2][1]);
        rightWallAttrsB[1].value.push_back(coordinatesRight[3][0]);
        rightWallAttrsB[1].value.push_back(coordinatesRight[3][1]);
        rightWallAttrsB[2].value.push_back(coordinatesRight[0][0]);
        rightWallAttrsB[2].value.push_back(coordinatesRight[0][1]);

        // Ceiling Attributes
        topWallAttrsA[0].value.push_back(coordinatesTop[0][0]);
        topWallAttrsA[0].value.push_back(coordinatesTop[0][1]);
        topWallAttrsA[1].value.push_back(coordinatesTop[1][0]);
        topWallAttrsA[1].value.push_back(coordinatesTop[1][1]);
        topWallAttrsA[2].value.push_back(coordinatesTop[2][0]);
        topWallAttrsA[2].value.push_back(coordinatesTop[2][1]);

        topWallAttrsB[0].value.push_back(coordinatesTop[2][0]);
        topWallAttrsB[0].value.push_back(coordinatesTop[2][1]);
        topWallAttrsB[1].value.push_back(coordinatesTop[3][0]);
        topWallAttrsB[1].value.push_back(coordinatesTop[3][1]);
        topWallAttrsB[2].value.push_back(coordinatesTop[0][0]);
        topWallAttrsB[2].value.push_back(coordinatesTop[0][1]);        

        // Assign and store used image
        static BufferImage myImage("checker.bmp");
        Attributes imageUniforms;
        imageUniforms.ptrImgs.push_back(&myImage);

        // Set image fragment shader
        FragmentShader fragImg;
        fragImg.FragShader = ImageFragShader;

        // Get the procedural grid
        FragmentShader otherFragImg;
        otherFragImg.FragShader = FragShaderUVwithoutImage;
                
        // Draw image triangles 
        DrawPrimitive(TRIANGLE, target, verticesImgA, imageAttributesA, &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, verticesImgB, imageAttributesB, &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, vertsBackA,   backWallAttrsA,   &imageUniforms, &otherFragImg);
        DrawPrimitive(TRIANGLE, target, vertsBackB,   backWallAttrsB,   &imageUniforms, &otherFragImg);
        DrawPrimitive(TRIANGLE, target, vertsLeftA,   leftWallAttrsA,   &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, vertsLeftB,   leftWallAttrsB,   &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, vertsRightA,  rightWallAttrsA,  &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, vertsRightB,  rightWallAttrsB,  &imageUniforms, &fragImg);        
        DrawPrimitive(TRIANGLE, target, vertsTopA,    topWallAttrsA,    &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, vertsTopB,    topWallAttrsB,    &imageUniforms, &fragImg);        
}

/************************************************
 * Demonstrate simple transformations for  
 * Project 05 in the vertex shader callback. 
 ***********************************************/
void TestVertexShader(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Interpolated color triangle
        *************************************************/
        Vertex colorTriangle[3];
        Attributes colorAttributes[3];
        colorTriangle[0] = { 350, 112, 1, 1};
        colorTriangle[1] = { 400, 200, 1, 1};
        colorTriangle[2] = { 300, 200, 1, 1};

        PIXEL colors[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your code for 'colorAttributes' goes here
        colorAttributes[0].value.push_back(1.0);
       	colorAttributes[0].value.push_back(0.0);
	colorAttributes[0].value.push_back(0.0);
	colorAttributes[1].value.push_back(0.0);
	colorAttributes[1].value.push_back(1.0);
	colorAttributes[1].value.push_back(0.0);
	colorAttributes[2].value.push_back(0.0);
	colorAttributes[2].value.push_back(0.0);
	colorAttributes[2].value.push_back(1.0);

        FragmentShader myColorFragShader;
	myColorFragShader.FragShader = ColorFragShader;

        Attributes colorUniforms;

        VertexShader myColorVertexShader;
        myColorVertexShader.VertShader = VertShader;

        /******************************************************************
        * TRANSLATE (move +100 in the X direction, +50 in the Y direction)
        *****************************************************************/
        // Your translating code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        colorUniforms.matrix.translate(100, 50, 0);

	DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /***********************************
         * SCALE (scale by a factor of 0.5)
         ***********************************/
        //Your scaling code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        colorUniforms.matrix.reset();
        colorUniforms.matrix.scale(0.5, 0.5, 0.5);

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /**********************************************
         * ROTATE 30 degrees in the X-Y plane around Z
         *********************************************/
        // Your rotation code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        colorUniforms.matrix.reset();
        colorUniforms.matrix.rotate(30);

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /*************************************************
         * SCALE-TRANSLATE-ROTATE in left-to-right order
         * the previous transformations concatenated.
         ************************************************/
	// Your scale-translate-rotation code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        colorUniforms.matrix.reset();
        colorUniforms.matrix.rotate(30);
        colorUniforms.matrix.translate(100, 50, 0); 
        colorUniforms.matrix.scale(0.5, 0.5, 0.5);
        
        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);	
}

/********************************************
 * Verify that the whole pipeline works. By
 * the end of week 07 you should be able to
 * run this code successfully.
 *******************************************/
void TestPipeline(Buffer2D<PIXEL> & target)
{
        // This is similar to TestDrawPerspectiveCorrect 
        // except that:
        //      1) perspective projection is expected from
        //         the programmer in the vertex shader.
        //      2) Clipping/normalization must be turned on.
        //      3) The ViewPort Transform must be applied.
        //      4) The Z-Buffer is incorporated into drawing.
        //      5) You may want to involve camera variables:
        //              i)   camYaw
        //              ii)  camPitch
        //              iii) camRoll, 
        //              iv)  camX
        //              v)   camY
        //              vi)  camZ
        //      To incorporate a view transform (add movement)
        
        static Buffer2D<double> zBuf(target.width(), target.height());
        // Will need to be cleared every frame, like the screen

        /**************************************************
        * 1. Image quad (2 TRIs) Code (texture interpolated)
        **************************************************/
        Vertex quad[] = { {-20,-20, 50, 1},
                          {20, -20, 50, 1},
                          {20, 20, 50, 1},
                          {-20,20, 50, 1}};

        Vertex verticesImgA[3];
        Attributes imageAttributesA[3];
        verticesImgA[0] = quad[0];
        verticesImgA[1] = quad[1];
        verticesImgA[2] = quad[2];

        Vertex verticesImgB[3];        
        Attributes imageAttributesB[3];
        verticesImgB[0] = quad[2];
        verticesImgB[1] = quad[3];
        verticesImgB[2] = quad[0];

        double coordinates[4][2] = { {0,0}, {1,0}, {1,1}, {0,1} };
        // Your texture coordinate code goes here for 'imageAttributesA, imageAttributesB'

        BufferImage myImage("checker.bmp");
        // Ensure the checkboard image is in this directory, you can use another image though

        Attributes imageUniforms;
        // Your code for the uniform goes here

        FragmentShader fragImg;
        // Your code for the image fragment shader goes here

        VertexShader vertImg;
        // Your code for the image vertex shader goes here
        // NOTE: This must include the at least the 
        // projection matrix if not more transformations 
                
        // Draw image triangle 
        DrawPrimitive(TRIANGLE, target, verticesImgA, imageAttributesA, &imageUniforms, &fragImg, &vertImg, &zBuf);
        DrawPrimitive(TRIANGLE, target, verticesImgB, imageAttributesB, &imageUniforms, &fragImg, &vertImg, &zBuf);

        // NOTE: To test the Z-Buffer additinonal draw calls/geometry need to be called into this scene
}

/****************************************
 * DETERMINANT
 * Find the determinant of a matrix with
 * components A, B, C, D from 2 vectors.
 ***************************************/
inline double determinant(const double & A, const double & B, const double & C, const double & D)
{
  return (A*D - B*C);
}

#endif
