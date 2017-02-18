struct Frame;
struct MeshContainer;

class SkeletalMesh
{
public:
	SkeletalMesh();
	SkeletalMesh(const char* fileName);
	~SkeletalMesh();
	bool Load(const char* fileName);
	void Update(float elapsedTime);
	void RenderSkeleton();
	void Render();
private:
	void UpdateFrame(const D3DXFRAME* base, const Matrix* parentMatrix);
	void SetupBone(Frame* frame, const Matrix* parentTransform);
	void DrawSkeleton(LPD3DXFRAME frame, const Matrix* position);
	void DrawFrame(LPD3DXFRAME frame);
	void DrawContainer(LPD3DXMESHCONTAINER container, LPD3DXFRAME frame);
	LPD3DXFRAME m_rootFrame;
	LPD3DXANIMATIONCONTROLLER m_aniCon;
	Matrix * m_boneMatrices;
	unsigned int m_maxBones;
	unsigned int m_numberOfAnims;
};