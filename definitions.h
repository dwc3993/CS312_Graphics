#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include <vector>

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/******************************************************
 * DEFINES:
 * Macros for universal variables/hook-ups.
 *****************************************************/
#define WINDOW_NAME "Pipeline"
#define S_WIDTH     512
#define S_HEIGHT    512
#define PIXEL       Uint32
#define ABS(in) (in > 0 ? (in) : -(in))
#define SWAP(TYPE, FIRST, SECOND) { TYPE tmp = FIRST; FIRST = SECOND; SECOND = tmp; }
#define MIN(A,B) A < B ? A : B
#define MAX(A,B) A > B ? A : B
#define MIN3(A,B,C) MIN((MIN(A,B)),C)
#define MAX3(A,B,C) MAX((MAX(A,B)),C)
#define X_KEY 0
#define Y_KEY 1
#define DEG_TO_RAD M_PI / 180

// Max # of vertices after clipping
#define MAX_VERTICES 8 

/******************************************************
 * Types of primitives our pipeline will render.
 *****************************************************/
enum PRIMITIVES 
{
    TRIANGLE,
    LINE,
    POINT
};

/****************************************************
 * Describes a geometric point in 3D space. 
 ****************************************************/
struct Vertex
{
    double x;
    double y;
    double z;
    double w;
};

/******************************************************
 * BUFFER_2D:
 * Used for 2D buffers including render targets, images
 * and depth buffers. Can be described as frames or 
 * 2D arrays ot type 'T' encapsulated in an object.
 *****************************************************/
template <class T>
class Buffer2D 
{
    protected:
        T** grid;
        int w;
        int h;

        // Private intialization setup
        void setupInternal()
        {
            // Allocate pointers for column references
            grid = (T**)malloc(sizeof(T*) * h);                
            for(int r = 0; r < h; r++)
            {
                grid[r] = (T*)malloc(sizeof(T) * w);
            }
        }

        // Empty Constructor
        Buffer2D()
        {}

    public:
        // Free dynamic memory
        ~Buffer2D()
        {
            // De-Allocate pointers for column references
            for(int r = 0; r < h; r++)
            {
                free(grid[r]);
            }
            free(grid);
        }

        // Size-Specified constructor, no data
        Buffer2D(const int & wid, const int & hgt)
        {
            h = hgt;
            w = wid;
            setupInternal();
            zeroOut();
        }

        // Assignment constructor
        Buffer2D& operator=(const Buffer2D & ib)
        {
            w = ib.width();
            h = ib.height();
            setupInternal();
            for(int r = 0; r < h; r++)
            {
                for(int c = 0; c < w; c++)
                {
                    grid[r][c] = ib[r][c];
                }
            }
        }

        // Set each member to zero 
        void zeroOut()
        {
            for(int r = 0; r < h; r++)
            {
                for(int c = 0; c < w; c++)
                {
                    grid[r][c] = 0;
                }
            }
        }

        // Width, height
        const int & width()  { return w; }
        const int & height() { return h; }

        // The frequented operator for grabbing pixels
        inline T* & operator[] (int i)
        {
            return grid[i];
        }
};


/****************************************************
 * BUFFER_IMAGE:
 * PIXEL (Uint32) specific Buffer2D class with .BMP 
 * loading/management features.
 ***************************************************/
class BufferImage : public Buffer2D<PIXEL>
{
    protected:       
        SDL_Surface* img;                   // Reference to the Surface in question
        bool ourSurfaceInstance = false;    // Do we need to de-allocate?

        // Private intialization setup
        void setupInternal()
        {
            // Allocate pointers for column references
            h = img->h;
            w = img->w;
            grid = (PIXEL**)malloc(sizeof(PIXEL*) * h);                

            PIXEL* row = (PIXEL*)img->pixels;
            row += (w*h);
            for(int i = 0; i < h; i++)
            {
                grid[i] = row;
                row -= w;                    
            }
        }

    public:
        // Free dynamic memory
        ~BufferImage()
        {
            // De-Allocate pointers for column references
            free(grid);

            // De-Allocate this image plane if necessary
            if(ourSurfaceInstance)
            {
                SDL_FreeSurface(img);
            }
        }

        // Assignment constructor
        BufferImage& operator=(const BufferImage & ib)
        {
            img = ib.img;
            w = ib.w;
            h = ib.h;
            ourSurfaceInstance = false;
            grid = (PIXEL**)malloc(sizeof(PIXEL*) * img->h);                
            for(int i = 0; i < img->w; i++)
            {
                grid[i] = ib.grid[i];
            }
        }

        // Constructor based on instantiated SDL_Surface
        BufferImage(SDL_Surface* src) 
        { 
            img = src; 
            w = src->w;
            h = src->h;
            ourSurfaceInstance = false;
            setupInternal();
        }

        // Constructor based on reading in an image - only meant for UINT32 type
        BufferImage(const char* path) 
        {
            ourSurfaceInstance = true;
            SDL_Surface* tmp = SDL_LoadBMP(path);      
            SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
            img = SDL_ConvertSurface(tmp, format, 0);
            SDL_FreeSurface(tmp);
            SDL_FreeFormat(format);
            setupInternal();
        }
};

/***************************************************
 * ATTRIBUTES (shadows OpenGL VAO, VBO)
 * The attributes associated with a rendered 
 * primitive as a whole OR per-vertex. Will be
 * designed/implemented by the programmer. 
 **************************************************/
class Attributes
{      
    public:

        // vectors to store as many attributes as necessary
        std::vector<double> value;
        std::vector<void*> ptrImgs;
        double matrix[4][4];
        PIXEL color;

        // Obligatory empty constructor
        Attributes() {}

        // Needed by clipping (linearly interpolated Attributes between two others)
        Attributes(const Attributes & first, const Attributes & second, const double & valueBetween)
        {
            // Your code goes here when clipping is implemented
        }

        Attributes &operator *= (const double rhs[][4]);// throw (const char *);
        Attributes &operator *= (const double rhs[]);// throw (const char *);
};

Attributes &Attributes::operator *= (const double rhs[][4]) //throw (const char *)
{
    double multiplied[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                multiplied[i][j] += matrix[k][j] * rhs[j][k];
            }
        }
    }

    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            matrix[i][j] = multiplied[i][j];


   return *this;
}

Attributes &Attributes::operator *= (const double rhs[]) // throw (const char *)
{
    double multiplied[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            multiplied[i] += matrix[i][j] * rhs[j];
        }
    }

   return *this;
}

/***************************************************
 * FragShader uses interpolated color values against
 * a percentage "weight" to compute correct color 
 **************************************************/
void ColorFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    // Output our shader color value, in this case simple colors
    PIXEL color = 0xff000000;
    color += (unsigned int)(vertAttr.value[0] *0xff) << 16;
    color += (unsigned int)(vertAttr.value[1] *0xff) << 8;
    color += (unsigned int)(vertAttr.value[2] *0xff) << 0;

    fragment = color;
}

/***************************************************
 * FragShader uses interpolated coordinate values 
 * against a percentage "weight" to compute correct 
 * coordinate 
 **************************************************/
void ImageFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    BufferImage* bf = (BufferImage*)uniforms.ptrImgs[0];
    int x = vertAttr.value[0] * (bf->width()-1);
    int y = vertAttr.value[1] * (bf->height()-1);

    fragment = (*bf)[y][x];
}

// Frag Shader for UV without image (due to SDL2 bug?)
void FragShaderUVwithoutImage(PIXEL & fragment, const Attributes & attributes, const Attributes & uniform)
{
    static int row = 8;
    static int col = 8;

    SDL_Event e;
    while(SDL_PollEvent(&e)) 
    {
            if(e.key.keysym.sym == 'w' && e.type == SDL_KEYDOWN) 
            {
                    row++;
            }
            if(e.key.keysym.sym == 'a' && e.type == SDL_KEYDOWN) 
            {
                    col--;
            }
            if(e.key.keysym.sym == 's' && e.type == SDL_KEYDOWN) 
            {
                    row--;
            }
            if(e.key.keysym.sym == 'd' && e.type == SDL_KEYDOWN) 
            {
                    col++;
            }
    }
    // Figure out which X/Y square our UV would fall on
    int xSquare = attributes.value[0] * row;
    int ySquare = attributes.value[1] * col;

	// Is the X square position even? The Y? 
    bool evenXSquare = (xSquare % 2) == 0;
    bool evenYSquare = (ySquare % 2) == 0;

    // Both even or both odd - red square
    if( (evenXSquare && evenYSquare) || (!evenXSquare && !evenYSquare) )
    {
        fragment = 0xff00ff00;
    }
    // One even, one odd - white square
    else
    {
        fragment = 0xff000000;
    }
}

// Example of a fragment shader
void DefaultFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    // Output our shader color value, in this case red
    fragment = 0xffff0000;
}

/*******************************************************
 * FRAGMENT_SHADER
 * Encapsulates a programmer-specified callback
 * function for shading pixels. See 'DefaultFragShader'
 * for an example. 
 ******************************************************/
class FragmentShader
{
    public:
 
        // Get, Set implicit
        void (*FragShader)(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms);

        // Assumes simple monotone RED shader
        FragmentShader()
        {
            FragShader = DefaultFragShader;
        }

        // Initialize with a fragment callback
        FragmentShader(void (*FragSdr)(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms))
        {
            setShader(FragSdr);
        }

        // Set the shader to a callback function
        void setShader(void (*FragSdr)(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms))
        {
            FragShader = FragSdr;
        }
};

// Example of a vertex shader
void DefaultVertShader(Vertex & vertOut, Attributes & attrOut, const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms)
{
    // Nothing happens with this vertex, attribute
    vertOut = vertIn;
    attrOut = vertAttr;
}

void VertShader(Vertex & vertOut, Attributes & attrOut, const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms)
{
    vertOut.x = uniforms.matrix[0][0] * vertIn.x + uniforms.matrix[0][1] * vertIn.y + 
                uniforms.matrix[0][2] * vertIn.z + uniforms.matrix[0][3] * vertIn.w;
    vertOut.y = uniforms.matrix[1][0] * vertIn.x + uniforms.matrix[1][1] * vertIn.y + 
                uniforms.matrix[1][2] * vertIn.z + uniforms.matrix[1][3] * vertIn.w;
    vertOut.z = uniforms.matrix[2][0] * vertIn.x + uniforms.matrix[2][1] * vertIn.y + 
                uniforms.matrix[2][2] * vertIn.z + uniforms.matrix[2][3] * vertIn.w;
    vertOut.w = uniforms.matrix[3][0] * vertIn.x + uniforms.matrix[3][1] * vertIn.y + 
                uniforms.matrix[3][2] * vertIn.z + uniforms.matrix[3][3] * vertIn.w;
    attrOut = vertAttr;

}

/**********************************************************
 * VERTEX_SHADER
 * Encapsulates a programmer-specified callback
 * function for transforming vertices and per-vertex
 * attributes. See 'DefaultVertShader' for a pass-through
 * shader example.
 *********************************************************/
class VertexShader
{
    public:
        // Get, Set implicit
        void (*VertShader)(Vertex & vertOut, Attributes & attrOut, const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms);

        // Assumes simple monotone RED shader
        VertexShader()
        {
            VertShader = DefaultVertShader;
        }

        // Initialize with a fragment callback
        VertexShader(void (*VertSdr)(Vertex & vertOut, Attributes & attrOut, const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms))
        {
            setShader(VertSdr);
        }

        // Set the shader to a callback function
        void setShader(void (*VertSdr)(Vertex & vertOut, Attributes & attrOut, const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms))
        {
            VertShader = VertSdr;
        }
};

void multiply(double lhs[][4], double rhs[][4]) {
    double multiplied[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                multiplied[i][j] += lhs[j][k] * rhs[k][j];
            }
        }
    }

    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            lhs[i][j] = multiplied[i][j];
}

void multiply(double lhs[][4], double rhs[]) {
    double multiplied[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            multiplied[i] += lhs[i][j] * rhs[j];
        }
    }

    for (int i = 0; i < 4; i++) {
            rhs[i] = multiplied[i];
    }
}

/***************************************************
 * Matrix
 **************************************************/
class Transform
{      
    public:

        //double matrix[4][4];

        // Obligatory empty constructor
        Transform() {}

        static void reset(Attributes* uniforms) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == j) {
                        uniforms->matrix[i][j] = 1;
                    } else {
                        uniforms->matrix[i][j] = 0;
                    }
                }
            }
        }

        static void translate(double x, double y, double z, Attributes* uniforms) {
            uniforms->matrix[0][3] += x;
            uniforms->matrix[1][3] += y;
            uniforms->matrix[2][3] += z;
            
            // double matrix[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
            // double newMatrix[4][4];

            // matrix[0][3] += x;
            // matrix[1][3] += y;
            // matrix[2][3] += z;

            // for (int i = 0; i < 4; i++) 
            //     for (int j = 0; j < 4; j++) 
            //         for (int k = 0; k < 4; k++) 
            //             newMatrix[i][j] += matrix[j][k] * uniforms->matrix[k][j];

            // for (int i = 0; i < 4; i++) 
            //     for (int j = 0; j < 4; j++) 
            //         uniforms->matrix[i][j] = newMatrix[i][j];
            
            //uniforms *= matrix;
            //multiply(uniforms->matrix, matrix);
        }

        static void rotate(double angle, Attributes* uniforms) {
            double radians = angle * DEG_TO_RAD;
            uniforms->matrix[0][0] = cos(radians);
            uniforms->matrix[0][1] = -sin(radians);
            uniforms->matrix[1][0] = sin(radians);
            uniforms->matrix[1][1] = cos(radians);
        }

        static void scale(double x, double y, double z, Attributes* uniforms) {
            uniforms->matrix[0][0] *= x;
            uniforms->matrix[1][1] *= y;
            uniforms->matrix[2][2] *= z;
        }
};	


// Stub for Primitive Drawing function
/****************************************
 * DRAW_PRIMITIVE
 * Prototype for main drawing function.
 ***************************************/
void DrawPrimitive(PRIMITIVES prim, 
                   Buffer2D<PIXEL>& target,
                   const Vertex inputVerts[], 
                   const Attributes inputAttrs[],
                   Attributes* const uniforms = NULL,
                   FragmentShader* const frag = NULL,
                   VertexShader* const vert = NULL,
                   Buffer2D<double>* zBuf = NULL);             
       
#endif
