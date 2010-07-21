
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
MD2Model::MD2Model() {
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;
	m_Model=0;
}

//---------------------------------------------------------------------------	MD2Model :: ~MD2Model
//
MD2Model::~MD2Model() {
	Release();
}

//---------------------------------------------------------------------------	MD2Model :: Release
//
void MD2Model::Release() {
	delete [] m_Verts;
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;

	MD2::Release( m_Model );

	m_Model = 0;
}

//---------------------------------------------------------------------------	MD2Model :: Load
//
bool MD2Model::Load(const char* filename) {
	Release();
	if( !(m_Model = MD2::Load(filename)) )
		return false;

	// allocate memory for vertex data
	m_Verts = new float[ 3 * m_Model->GetNumVerts() ];
			
	return true;
}

//---------------------------------------------------------------------------	MD2Model :: Update
//
void MD2Model::Update(float dt) {
	if(!m_Model)
		return;

	m_AnimTime += dt;

	// get ptr to anim ref
	const MD2::AnimRef* panimref = m_Model->GetAnim(m_CurrentAnim);

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
	const MD2::frame* pf1 = m_Model->GetFrame(f1);
	const MD2::frame* pf2 = m_Model->GetFrame(f2);
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
	unsigned short num_verts = m_Model->GetNumVerts();
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
	if(!m_Model)
		return;

	// if we have gl Commands in the file, use those for they are 
	// slightly faster (err, actually, that's not always the case)
	//
	if( !(MD2_ALWAYS_GL_TRIANGLES) && 
		 m_Model->GetModel()->numGlCommands > 1 ) 
	{

		// the Md2 stores how many DWORDS (4 byte chunks) are used
		// We want this in bytes so we can figure out the total size
		// of the gl command section
		// 
//		int gl_command_data_size = 4 * m_Model->GetModel()->numGlCommands;

		// use pointer to walk through the data
		unsigned char* ptr = m_Model->GetCommandsStart();
		unsigned char* end = m_Model->GetCommandsEnd();

		// loop till end
		while(ptr != end) 
		{
			// get the command list for this strip or fan
			MD2::glCommandList* pCommand = ((MD2::glCommandList*)((void*)ptr));
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
				MD2::glCommandVertex* pStart=pCommand->verts;
				MD2::glCommandVertex* pEnd  = pStart + num;
				for( ; pStart != pEnd; ++pStart )
				{
					glTexCoord2fv(pStart->data);
					glVertex3fv(m_Verts + 3*pStart->vertexIndex);
				}
			glEnd();

			// move iterator to next command
			ptr += (4 + sizeof(MD2::glCommandVertex)*num);
			
		}
	}

	// otherwise use triangle list
	else
	{
		// since the UV data in an md2 file is specified as shorts,
		// scale the texture matrix stack accordingly to get the texture
		// coords within the 0 to 1 range
		//
		float uv_scale_s = 1.0f / m_Model->GetModel()->skinWidth;
		float uv_scale_t = 1.0f / m_Model->GetModel()->skinHeight;

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glScalef(uv_scale_s,uv_scale_t,1.0f);
		glMatrixMode(GL_MODELVIEW);

		// draw each triangle
		glBegin(GL_TRIANGLES);
			// get num of triangles in mesh
			unsigned short nTris = m_Model->GetNumTriangles();

			// get point to triangle
			const MD2::triangle* pt = m_Model->GetTriangles();

			// loop through each triangle 
			for(unsigned short i=0;i!=nTris;++i) 
			{
				// loop through each vertex on the triangle
				for(unsigned int j=0;j!=3;++j) 
				{
					const float* pvertex = m_Verts + 3*pt[i].vertexIndices[j];
					const MD2::uv*    puv     = m_Model->GetTexCoords() + pt[i].textureIndices[j];

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
		return m_Model->GetFrame( m_Model->GetAnim(idx)->m_FrameStart )->name;
	return 0;
}

//---------------------------------------------------------------------------	MD2Model :: GetNumAnims
//
unsigned short MD2Model::GetNumAnims() const {
	return static_cast<unsigned short>(m_Model->GetNumAnims());
}

//---------------------------------------------------------------------------	MD2Model :: GetDataSize
//
unsigned int MD2Model::GetDataSize() const {
	return static_cast<unsigned int>(
		sizeof(MD2Model) + 
		sizeof(float)*3*m_Model->GetNumVerts());
}
