#define SDL_MAIN_HANDLED
#include "SDL2\SDL.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h" 
#include <chrono>
#include <ctime>

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
 * X, Y, Z, handy enums
 ***************************************************/
enum DIMENSION
{
    X = 0,
    Y = 1,
    Z = 2
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
struct camControls
{
	double x = 0;
	double y = 0;
	double z = 0;
	double yaw = 0;
	double roll = 0;
	double pitch = 0;
};
camControls myCam;
camControls myCam1;
camControls myCam2;
camControls myCam3;
// /******************************************************
//  * BUFFER_2D:
//  * Used for 2D buffers including render targets, images
//  * and depth buffers. Can be described as frames or 
//  * 2D arrays ot type 'T' encapsulated in an object.
//  *****************************************************/
// template <class T>
// class Buffer2D 
// {
//     protected:
//         T** grid;
//         int w;
//         int h;

//         // Private intialization setup
//         void setupInternal()
//         {
//             // Allocate pointers for column references
//             grid = (T**)malloc(sizeof(T*) * h);                
//             for(int r = 0; r < h; r++)
//             {
//                 grid[r] = (T*)malloc(sizeof(T) * w);
//             }
//         }

//         // Empty Constructor
//         Buffer2D()
//         {}

//     public:
//         // Free dynamic memory
//         ~Buffer2D()
//         {
//             return;
//             // De-Allocate pointers for column references
//             for(int r = 0; r < h; r++)
//             {
//                 free(grid[r]);
//             }
//             free(grid);
//         }

//         // Size-Specified constructor, no data
//         Buffer2D(const int & wid, const int & hgt)
//         {
//             h = hgt;
//             w = wid;
//             setupInternal();
//             zeroOut();
//         }

//         // Assignment constructor
//         Buffer2D& operator=(const Buffer2D & ib)
//         {
//             w = ib.width();
//             h = ib.height();
//             setupInternal();
//             for(int r = 0; r < h; r++)
//             {
//                 for(int c = 0; c < w; c++)
//                 {
//                     grid[r][c] = ib[r][c];
//                 }
//             }
//         }

//         // Set each member to zero 
//         void zeroOut()
//         {
//             for(int r = 0; r < h; r++)
//             {
//                 for(int c = 0; c < w; c++)
//                 {
//                     grid[r][c] = 0;
//                 }
//             }
//         }

//         // Width, height
//         const int & width()  { return w; }
//         const int & height() { return h; }

//         // The frequented operator for grabbing pixels
//         inline T* & operator[] (int i)
//         {
//             return grid[i];
//         }
// };


// /****************************************************
//  * BUFFER_IMAGE:
//  * PIXEL (Uint32) specific Buffer2D class with .BMP 
//  * loading/management features.
//  ***************************************************/
// class BufferImage : public Buffer2D<PIXEL>
// {
//     protected:       
//         SDL_Surface* img;                   // Reference to the Surface in question
//         bool ourSurfaceInstance = false;    // Do we need to de-allocate?

//         // Private intialization setup
//         void setupInternal()
//         {
//             // Allocate pointers for column references
//             h = img->h;
//             w = img->w;
//             grid = (PIXEL**)malloc(sizeof(PIXEL*) * h);                

//             PIXEL* row = (PIXEL*)img->pixels;
//             row += (w*h);
//             for(int i = 0; i < h; i++)
//             {
//                 grid[i] = row;
//                 row -= w;                    
//             }
//         }

//     public:
//         // Free dynamic memory
//         ~BufferImage()
//         {
//             return;
//             // De-Allocate pointers for column references
//             free(grid);

//             // De-Allocate this image plane if necessary
//             if(ourSurfaceInstance)
//             {
//                 SDL_FreeSurface(img);
//             }
//         }

//         // Assignment constructor
//         BufferImage& operator=(const BufferImage & ib)
//         {
//             img = ib.img;
//             w = ib.w;
//             h = ib.h;
//             ourSurfaceInstance = false;
//             grid = (PIXEL**)malloc(sizeof(PIXEL*) * img->h);                
//             for(int i = 0; i < img->w; i++)
//             {
//                 grid[i] = ib.grid[i];
//             }
//         }

//         // Constructor based on instantiated SDL_Surface
//         BufferImage(SDL_Surface* src) 
//         { 
//             img = src; 
//             w = src->w;
//             h = src->h;
//             ourSurfaceInstance = false;
//             setupInternal();
//         }

//         // Constructor based on reading in an image - only meant for UINT32 type
//         BufferImage(const char* path) 
//         {
//             ourSurfaceInstance = true;
//             SDL_Surface* tmp = SDL_LoadBMP(path);      
//             SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
//             img = SDL_ConvertSurface(tmp, format, 0);
//             SDL_FreeSurface(tmp);
//             SDL_FreeFormat(format);
//             setupInternal();
//         }
// };
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
        bool baseAllocated = false;
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
	    baseAllocated = true;
        }

        // Empty Constructor
	Buffer2D() {}
		  

    public:
        // Free dynamic memory
        ~Buffer2D()
        {
	  // De-Allocate pointers for column references
	  if(baseAllocated)	    
	    {
	      for(int r = 0; r < h; r++)
		{
		  free(grid[r]);
		}
	      free(grid);

	    }
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


// Struct used for reading in RGB values from a bitmap file
struct bmpRGB
{
  unsigned char b;
  unsigned char g; 
  unsigned char r;
};

// The portion of the Bitmap header we want to read
struct bmpLayout
{
  int offset;
  int headerSize;
  int width;
  int height;
  unsigned short colorPlanes;
  unsigned short bpp;
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
        bool ourSurfaceInstance = false;    // Do we need to de-allocate the SDL2 reference?
    	bool ourBufferData = false;         // Are we using the non-SDL2 allocated memory

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

    private:

        // Non-SDL2 24BPP, 2^N dimensions BMP reader
        bool readBMP(const char* fileName)
        {
            // Read in Header - check signature
            FILE * fp = fopen(fileName, "rb");	    
            char signature[2];
            fread(signature, 1, 2, fp);
            if(!(signature[0] == 'B' && signature[1] == 'M'))
            {
                printf("Invalid header for file: \"%c%c\"", signature[0], signature[1]);
                return 1;
            }

            // Read in BMP formatting - verify type constraints
            bmpLayout layout;
            fseek(fp, 8, SEEK_CUR);
            fread(&layout, sizeof(layout), 1, fp);
            if(layout.width % 2 != 0 || layout.width <= 4)
            {
                printf("Size Width MUST be a power of 2 larger than 4; not %d\n", w);
                return false;		
            }
            if(layout.bpp != 24)
            {
                printf("Bits per pixel of image must be 24; not %d\n", layout.bpp);
                return false;
            }

            // Copy W+H information
            w = layout.width;
            h = layout.height;
    
            // Initialize internal pointers/memory
            grid = (PIXEL**)malloc(sizeof(PIXEL*) * h);
            for(int y = 0; y < h; y++) grid[y] = (PIXEL*)malloc(sizeof(PIXEL) * w);

            // Advance to beginning of pixel data, read values in
            bmpRGB* pixel = (bmpRGB*)malloc(sizeof(bmpRGB)*w*h);
            fseek(fp, layout.offset, SEEK_SET);  	
            fread(pixel, sizeof(bmpRGB), w*h, fp);

            // Convert 24-bit RGB to 32-bit ARGB
            bmpRGB* pixelPtr = pixel;
            PIXEL* out = (PIXEL*)malloc(sizeof(PIXEL)*w*h);
            for(int y = 0; y < h; y++)
            {
                for(int x = 0; x < w; x++)
                {
                    grid[y][x] = 0xff000000 + 
                                 ((pixelPtr->r) << 16) +
                                 ((pixelPtr->g) << 8) +
                                 ((pixelPtr->b));
                                 ++pixelPtr;
                }
            }
    
            // Release 24-Bit buffer, release file
            free(pixel);
            fclose(fp); 
            return true;
        }

    public:
        // Free dynamic memory
        ~BufferImage()
        {
            // De-Allocate non-SDL2 image data
            if(ourBufferData)
            {
	        free(grid);
		return;
            }

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
	  ourSurfaceInstance = false;
	  if(!readBMP(path))
	    {
	      return;
	    }	  
	  ourBufferData = true;
        }
};

    class Matrix
    {
        private:
           int row;
           int col;
        public:
           double container[4][4] = {{1,0,0,0},
                               {0,1,0,0},
                               {0,0,1,0},
                               {0,0,0,1}};
        
        Matrix()
        {
            row = 4;
            col = 4;
           this->container[0][0] = 1;
           this->container[1][1] = 1; 
           this->container[2][2] = 1; 
           this->container[0][1] = 0;
           this->container[0][2] = 0; 
           this->container[0][3] = 0; 
           this->container[1][0] = 0;
           this->container[1][2] = 0; 
           this->container[1][3] = 0; 
           this->container[2][0] = 0;
           this->container[2][1] = 0; 
           this->container[2][3] = 0; 
           this->container[3][0] = 0;
           this->container[3][1] = 0;
           this->container[3][2] = 0; 
           this->container[3][3] = 1; 
            
        }

        Matrix scaleMatrix(double x, double y, double z)
        {
           this->container[0][0] = x;
           this->container[1][1] = y; 
           this->container[2][2] = z; 
           this->container[0][1] = 0;
           this->container[0][2] = 0; 
           this->container[0][3] = 0; 
           this->container[1][0] = 0;
           this->container[1][2] = 0; 
           this->container[1][3] = 0; 
           this->container[2][0] = 0;
           this->container[2][1] = 0; 
           this->container[2][3] = 0; 
           this->container[3][0] = 0;
           this->container[3][1] = 0;
           this->container[3][2] = 0; 
           this->container[3][3] = 1; 
           return (*this);
        }        
        
        Matrix transMatrix(double x, double y, double z)
        {
           this->container[0][3] = x;
           this->container[1][3] = y; 
           this->container[2][3] = z; 
           this->container[3][3] = 1;
           this->container[0][0] = 1; 
           this->container[0][1] = 0; 
           this->container[0][2] = 0;
           this->container[1][0] = 0; 
           this->container[1][1] = 1; 
           this->container[1][2] = 0; 
           this->container[2][0] = 0;
           this->container[2][1] = 0; 
           this->container[2][2] = 1; 
           this->container[3][0] = 0;
           this->container[3][1] = 0; 
           this->container[3][2] = 0; 
           return (*this);
        }

        Matrix rotatZMatrix(double degrees)
        {
           double sina = sin(degrees * M_PI/180); 
           double cosa = cos(degrees * M_PI/180); 
           this->container[0][0] = cosa;
           this->container[0][1] = -sina; 
           this->container[1][0] = sina; 
           this->container[1][1] = cosa; 
           this->container[0][2] = 0;
           this->container[0][3] = 0;
           this->container[1][2] = 0;
           this->container[1][3] = 0; 
           this->container[2][0] = 0; 
           this->container[2][1] = 0; 
           this->container[2][2] = 1; 
           this->container[2][3] = 0; 
           this->container[3][1] = 0; 
           this->container[3][2] = 0; 
           this->container[3][3] = 1; 
           this->container[3][0] = 0; 
           return (*this);
        } 
      
       double* operator[](int i)
       {
           return this->container[i];

       }
       const double* operator[](int i) const
       {
           return this->container[i];

       }

    };      

    Matrix operator * (const Matrix &lhs, const Matrix &rhs)
    {
        Matrix m;
        m.container[0][0] = 1;
        m.container[1][1] = 1; 
        m.container[2][2] = 1; 
        m.container[0][1] = 0;
        m.container[0][2] = 0; 
        m.container[0][3] = 0; 
        m.container[1][0] = 0;
        m.container[1][2] = 0; 
        m.container[1][3] = 0; 
        m.container[2][0] = 0;
        m.container[2][1] = 0; 
        m.container[2][3] = 0; 
        m.container[3][0] = 0;
        m.container[3][1] = 0;
        m.container[3][2] = 0; 
        m.container[3][3] = 1; 
   
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                m.container[i][j] = lhs.container[i][0] * rhs.container[0][j] + lhs.container[i][1] * rhs.container[1][j] + lhs.container[i][2] * rhs.container[2][j] + lhs.container[i][3] * rhs.container[3][j]; 
            }
        
        return m;
    }
    Vertex operator *(Matrix lhs, Vertex rhs)
    {
          Vertex v;
          v.x = (lhs.container[0][0] * rhs.x) + (lhs.container[0][1] * rhs.y)+ (lhs.container[0][2] * rhs.z)+ (lhs.container[0][3] * rhs.w);
          v.y = (lhs.container[1][0] * rhs.x) + (lhs.container[1][1] * rhs.y)+ (lhs.container[1][2] * rhs.z)+ (lhs.container[1][3] * rhs.w);
          v.z = (lhs.container[2][0] * rhs.x) + (lhs.container[2][1] * rhs.y)+ (lhs.container[2][2] * rhs.z)+ (lhs.container[2][3] * rhs.w);
          v.w = (lhs.container[3][0] * rhs.x) + (lhs.container[3][1] * rhs.y)+ (lhs.container[3][2] * rhs.z)+ (lhs.container[3][3] * rhs.w);
        return v;
    }

Matrix rotate4x4(const DIMENSION & dim, const double & degs)
{
  Matrix tr;
  double rads = degs * M_PI / 180.0;
  double cosT = cos(rads);
  double sinT = sin(rads);

  tr[0][0] = 1;
  tr[0][1] = 0;
  tr[0][2] = 0;
  tr[0][3] = 0;
  tr[1][0] = 0;
  tr[1][1] = 1;
  tr[1][2] = 0;
  tr[1][3] = 0;
  tr[2][0] = 0;
  tr[2][1] = 0;
  tr[2][2] = 1;
  tr[2][3] = 0;
  tr[3][0] = 0;
  tr[3][1] = 0;
  tr[3][2] = 0;
  tr[3][3] = 1;

  switch(dim)
    {
    case X:
      tr[1][1] = cosT;
      tr[1][2] = -sinT;
      tr[2][1] = sinT;
      tr[2][2] = cosT;
      break;
    case Y:
      tr[0][0] = cosT;
      tr[0][2] = sinT;
      tr[2][0] = -sinT;
      tr[2][2] = cosT;
      break;
    case Z:
      tr[0][0] = cosT;
      tr[0][1] = -sinT;
      tr[1][0] = sinT;
      tr[1][1] = cosT;
      break;
    }

  return tr;
}
    Matrix perspective4x4(const double & fovYDegrees, const double & aspectRatio, 
			 const double & near, const double & far)
{
		Matrix rt;
		
		double top = near * tan((fovYDegrees * M_PI) / 180.0 /2.0);
		double right = aspectRatio * top;
		
		rt[0][0] = near / right;
		rt[0][2] = 0;
		rt[0][1] = 0;
		rt[0][3] = 0;
		
		rt[1][0] = 0;
		rt[1][1] = near / top;
		rt[1][2] = 0;
		rt[1][3] = 0;
		
		rt[2][0] = 0;
		rt[2][1] = 0;
		rt[2][2] = (far + near) / (far - near);
		rt[2][3] = (-2 * far * near) / (far - near);
		
		rt[3][0] = 0;
		rt[3][1] = 0;
		rt[3][2] = 1;
		rt[3][3] = 0;
		
		return rt;		
}

 Matrix ortho(const double & top, const double & right, 
			 const double & near, const double & far)
{
     Matrix rt;
		
		rt[0][0] = 2/(2 * right); 
		rt[0][2] = 0;
		rt[0][1] = 0;
		rt[0][3] = 0;
		
		rt[1][0] = 0;
		rt[1][1] = 2/(2* top);
		rt[1][2] = 0;
		rt[1][3] = 0;
		
		rt[2][0] = 0;
		rt[2][1] = 0;
		rt[2][2] = (2 / (far - near));
		rt[2][3] = (-(far + near)/(far - near));
		
		rt[3][0] = 0;
		rt[3][1] = 0;
		rt[3][2] = 0;
		rt[3][3] = 1;
		
		return rt;		
     
}
Matrix camera4x4(const double & offX, const double & offY, const double & offZ,
					 const double & yaw, const double & pitch, const double & roll)
{
	Matrix trans = trans.transMatrix(-offX, -offY, -offZ);
	Matrix rotX = rotate4x4(X, -pitch);
	Matrix rotY = rotate4x4(Y, -yaw);
	
	Matrix rt = rotX * rotY * trans;
	return rt;	
}
    //  
    // Combine two datatypes in one
union attrib
{
  double d;
  void* ptr;
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
        // Members
        PIXEL color;
    	int numMembers = 0;
        attrib arr[16];
        Matrix matrix;
        // Obligatory empty constructor
        Attributes() {numMembers = 0;}

        // Needed by clipping (linearly interpolated Attributes between two others)
        Attributes(const Attributes & first, const Attributes & second, const double & along)
        {
				numMembers = first.numMembers;
				for(int i = 0; i < numMembers; i++)
				{
					arr[i].d = (first[i].d) + ((second[i].d - first[i].d) * along);
				}
        }

        // Const Return operator
        const attrib & operator[](const int & i) const
        {
            return arr[i];
        }

        // Return operator
        attrib & operator[](const int & i) 
        {
            return arr[i];
        }

        // Insert Double Into Container
        void insertDbl(const double & d)
        {
            arr[numMembers].d = d;
            numMembers += 1;
        }
    
        // Insert Pointer Into Container
        void insertPtr(void * ptr)
        {
            arr[numMembers].ptr = ptr;
            numMembers += 1;
        }
};	


// Example of a fragment shader
void DefaultFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    // Output our shader color value, in this case red
    fragment = 0xffff0000;
}
void ColorFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    PIXEL color = 0xff000000;
    color += (unsigned int)(vertAttr[0].d *0xff) << 16;
    color += (unsigned int)(vertAttr[1].d *0xff) << 8;
    color += (unsigned int)(vertAttr[2].d *0xff) << 0;

    fragment = color;
}

void ImageFragShader(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    BufferImage* ptr = (BufferImage*)uniforms[0].ptr;

    int x = vertAttr[0].d * (ptr->width() - 1);
    int y = vertAttr[1].d * (ptr->height() - 1);

    fragment = (*ptr)[y][x];
}
// Frag Shader for UV without image (due to SDL2 bug?)
void FragShaderUVwithoutImage(PIXEL & fragment, const Attributes & attributes, const Attributes & uniform)
{
    // Figure out which X/Y square our UV would fall on
    int xSquare = attributes[0].d * 8;
    int ySquare = attributes[1].d * 8;

	// Is the X square position even? The Y? 
    bool evenXSquare = (xSquare % 2) == 0;
    bool evenYSquare = (ySquare % 2) == 0;

    // Both even or both odd - red square
    if( (evenXSquare && evenYSquare) || (!evenXSquare && !evenYSquare) )
    {
        fragment = 0xffff0000;
    }
    // One even, one odd - white square
    else
    {
        fragment = 0xffffffff;
    }
}


// This shader will apply an orange tint to an image, brightening the dark and darkening the light.
void FragShaderOrangeNegative(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    BufferImage* ptr = (BufferImage*)uniforms[0].ptr;

    int x = (vertAttr[0].d) * (ptr->width() - 1);
    int y = (vertAttr[1].d) * (ptr->height() - 1);

    fragment = ((*ptr)[y][x] * 0x00ff) << 8;
}

// This shader generates random colors at every pixel at every call using the random number function.
void FragShaderStatic(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    BufferImage* ptr = (BufferImage*)uniforms[0].ptr;

    double r = rand();

    int x = vertAttr[0].d * (ptr->width() - 1);
    int y = vertAttr[1].d * (ptr->height() - 1);

    fragment = ((*ptr)[y][x] * r);
}

// This shader will generate a "pattern" using real-time seconds. It follows a sort of pattern but it will never be the
// same twice without time travel.
void FragShaderPattern(PIXEL & fragment, const Attributes & vertAttr, const Attributes & uniforms)
{
    time_t seconds = time(NULL);

    PIXEL color = 0xff000000;
    color += (unsigned int)(vertAttr[1].d *0xff) << 16;
    color += (unsigned int)(vertAttr[1].d *0xff) << 8;
    color += (unsigned int)(vertAttr[1].d *0xff) << 0;


    fragment = color * (seconds / 3);
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
void vShader(Vertex & vertOut, Attributes & attrOut,const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms)
{
}

void vShader2(Vertex & vertOut, Attributes & attrOut,const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms)
{
    Matrix* model = (Matrix*)uniforms[1].ptr;
    Matrix* view = (Matrix*)uniforms[2].ptr;
    Matrix* proj = (Matrix*)uniforms[3].ptr;
    vertOut = (*proj) * (*view) * (*model) *vertIn;
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
            // VertSkhader = vShader;
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
//         void vShader(void(*VertSdr)(Vertex & vertOut, Attributes & attrOut,const Vertex & vertIn, const Attributes & vertAttr, const Attributes & uniforms))
//         {
//            Matrix matrixOut = *uniforms.matrix;
//            matrixOut * vertIn;
//            vertOut.x = matrixOut.matrix[0][0];
//            vertOut.y = matrixOut.matrix[1][1];
//            vertOut.z = matrixOut.matrix[2][2];
//            vertOut.w = matrixOut.matrix[3][3];
//            attrOut = vertAttr;

// }
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