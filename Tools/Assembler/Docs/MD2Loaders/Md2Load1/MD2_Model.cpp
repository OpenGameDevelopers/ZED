
#include "MD2_Model.h"


#define MD2_FRAME_RATE (1.0f/MD2_FRAMES_PER_SEC)

//---------------------------------------------------------------------------	LERP
//
template< typename InType >
inline void LERP(float out[],const InType a[],const InType b[],const float interp_t) {
	float inv_interp_t = 1.0f - interp_t;
	out[0] = a[0]*inv_interp_t + b[0]*interp_t;
	out[1] = a[1]*inv_interp_t + b[1]*interp_t;
	out[2] = a[2]*inv_interp_t + b[2]*interp_t;
}

//---------------------------------------------------------------------------	MD2Model :: MD2Model
//
MD2Model::MD2Model() : MD2() {
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;
	m_NumAnims=0;
}

//---------------------------------------------------------------------------	MD2Model :: ~MD2Model
//
MD2Model::~MD2Model() {
	Release();
}

//---------------------------------------------------------------------------	MD2Model :: Release
//
void MD2Model::Release() {
	m_Anims.clear();
	delete [] m_Verts;
	delete [] m_data;
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;
	m_NumAnims=0;
	m_data=0;
}

//---------------------------------------------------------------------------	MD2Model :: Load
//
bool MD2Model::Load(const char* filename) {
	Release();
	if( !MD2::Load(filename) )
		return false;

	// allocate memory for vertex data
	m_Verts = new float[ 3 * GetNumVerts() ];
			
	// get number of frames
	unsigned int nf = GetModel()->numFrames;

	// quick name lookup for frame - num
	std::string firstName="";

	// used to build up references to animation cycles
	AnimRef ref;
	ref.m_FrameStart=0;
	ref.m_FrameCount=0;

	// loop through all frames to find counts
	for( unsigned int i=0 ; i != nf; ++i )
	{
		const frame* thisFrame = GetFrame(i);

		if(firstName=="" || strncmp(firstName.c_str(),thisFrame->name,firstName.size()) != 0 )
		{
			firstName = thisFrame->name;
			if(firstName[firstName.size()-2]=='0')
				firstName.resize(firstName.size()-2);
			else 
				firstName.resize(firstName.size()-1);

			if(ref.m_FrameCount!=0) {
				m_Anims.push_back(ref);
				ref.m_FrameStart=i;
				ref.m_FrameCount=0;
			}
		}
		++ref.m_FrameCount;
	}

	// make sure we don't miss the last cycle
	if(ref.m_FrameCount!=0) 
		m_Anims.push_back(ref);

	return true;
}

//---------------------------------------------------------------------------	MD2Model :: Update
//
void MD2Model::Update(float dt) {
	if(!m_data)
		return;

	m_AnimTime += dt;

	// get ptr to anim ref
	const AnimRef* panimref = &(m_Anims[m_CurrentAnim]);

	// get total length of anim
	float anim_len = (panimref->m_FrameCount*MD2_FRAME_RATE);

	// ensure our anim time does not go above the max time
	while(m_AnimTime > anim_len) {
		m_AnimTime -= anim_len;
	}

	// need to find the two frames between which we are animating
	int f1 = panimref->m_FrameStart;
	float time_between_frames = m_AnimTime;

	// find the first frame id, and calc time between frames
	while( time_between_frames > MD2_FRAME_RATE ) {
		time_between_frames -= MD2_FRAME_RATE;
		++f1;
	}


	// second frame is either first in cycle (if looping) or
	// the next frame. 
	int f2 = panimref->m_FrameStart;
	if( f1 < (panimref->m_FrameStart+panimref->m_FrameCount-1) ) {
		f2 = f1+1;
	}

	// get pointers to frames
	const frame* pf1 = GetFrame(f1);
	const frame* pf2 = GetFrame(f2);
	assert(pf1 && pf2);

	// scale this to 1 to zero for calculation
	float interp_t = time_between_frames/MD2_FRAME_RATE;

	// need to interpolate the scale and translation values for the keys
	float scale[3];
	float translate[3];

	// LERP scale
	LERP(scale,pf1->scale,pf2->scale,interp_t);

	// LERP translation
	LERP(translate,pf1->translate,pf2->translate,interp_t);

	// LERP each vertex and transform to final correct size
	unsigned short num_verts = GetNumVerts();
	unsigned int out_vert_idx=0;
	for(unsigned int i=0; i!= num_verts; ++i, out_vert_idx+=3 )
	{
		// temp buffer
		float temp[3];

		//LERP vertex
		LERP(temp,pf1->vertices[i].vert,pf2->vertices[i].vert,interp_t);

		// get out vertex
		float *vert = m_Verts + out_vert_idx;

		// scale vertex and flip coords
		vert[0] =    temp[0]*scale[0] + translate[0];
		vert[2] =  -(temp[1]*scale[1] + translate[1]);
		vert[1] =    temp[2]*scale[2] + translate[2];

	}			
}

//---------------------------------------------------------------------------	MD2Model :: Render
//
void MD2Model::Render() const {

	// ensure valid model loaded
	if(!m_data)
		return;

	// if we have gl Commands in the file, use those for they are 
	// slightly faster
	//
	if( !(MD2_ALWAYS_GL_TRIANGLES) && 
		 GetModel()->numGlCommands > 1 ) 
	{

		// the Md2 stores how many DWORDS (4 byte chunks) are used
		// We want this in bytes so we can figure out the total size
		// of the gl command section
		// 
		int gl_command_data_size = 4 * GetModel()->numGlCommands;

		// use pointer to walk through the data
		unsigned char* ptr = m_data + GetModel()->offsetGlCommands;
		unsigned char* end = ptr + gl_command_data_size;

		// loop till end
		while(ptr != end) 
		{
			// get the command list for this strip or fan
			glCommandList* pCommand = ((glCommandList*)((void*)ptr));
			int num = pCommand->num;

			// ignore if zero
			if(num == 0) {
				return; /*
				ptr += 4;
				continue;*/
			}
			
			// the primitive type to draw
			GLenum PrimitiveType = GL_TRIANGLE_STRIP;

			// if num is negative, we have to draw a triangle fan
			if(num<0) {
				PrimitiveType = GL_TRIANGLE_FAN;
				num = -num;
			}
			
			// start drawing
			glBegin(PrimitiveType);

				// iterate through the openGL command vertices.
				// these basically just contain a vertex index and 
				// the UV coord data for this vertex
				//
				glCommandVertex* pStart=pCommand->verts;
				glCommandVertex* pEnd  = pStart + num;
				for( ; pStart != pEnd; ++pStart )
				{
					glTexCoord2fv(pStart->data);
					glVertex3fv(m_Verts + 3*pStart->vertexIndex);
				}
			glEnd();

			// move iterator to next command
			ptr += (4 + sizeof(glCommandVertex)*num);
			
		}
	}

	// otherwise use triangle list
	else
	{
		// since the UV data in an md2 file is specified as shorts,
		// scale the texture matrix stack accordingly to get the texture
		// coords within the 0 to 1 range
		//
		float uv_scale_s = 1.0f / GetModel()->skinWidth;
		float uv_scale_t = 1.0f / GetModel()->skinHeight;

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glScalef(uv_scale_s,uv_scale_t,1.0f);
		glMatrixMode(GL_MODELVIEW);

		// draw each triangle
		glBegin(GL_TRIANGLES);
			// get num of triangles in mesh
			unsigned short nTris = GetNumTriangles();

			// get point to triangle
			const triangle* pt = GetTriangles();

			// loop through each triangle 
			for(unsigned short i=0;i!=nTris;++i) 
			{
				// loop through each vertex on the triangle
				for(unsigned int j=0;j!=3;++j) 
				{
					const float* pvertex = m_Verts + 3*pt[i].vertexIndices[j];
					const uv*    puv     = GetTexCoords() + pt[i].textureIndices[j];

					// set UV
					glTexCoord2sv(puv->data);

					// draw vertex
					glVertex3fv(pvertex);
				}
			}
		glEnd();
		
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
}


//---------------------------------------------------------------------------	MD2Model :: SetAnim
//
void MD2Model::SetAnim(unsigned short idx) {
	if(idx < GetNumAnims()) {
		m_CurrentAnim = idx;
		m_AnimTime=0;
	}
}

//---------------------------------------------------------------------------	MD2Model :: GetAnimName
//
const char* MD2Model::GetAnimName(unsigned short idx) const {
	if(idx<GetNumAnims())
		return GetFrame( m_Anims[idx].m_FrameStart )->name;
	return 0;
}

//---------------------------------------------------------------------------	MD2Model :: GetNumAnims
//
unsigned short MD2Model::GetNumAnims() const {
	return static_cast<unsigned short>(m_Anims.size());
}

//---------------------------------------------------------------------------	MD2Model :: GetDataSize
//
unsigned int MD2Model::GetDataSize() const {
	return static_cast<unsigned int>(
		sizeof(MD2Model) + data_size +
		m_Anims.size() * sizeof(AnimRef) +
		sizeof(float)*3*GetNumVerts());
}
