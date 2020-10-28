#ifndef CCAMERA2D_H
#define CCAMERA2D_H


class CCamera2D
{
    public:
        CCamera2D();
        CCamera2D(int x,int y);
        virtual ~CCamera2D();
        int GetCamera2DX(){return m_camera_x;};
        int GetCamera2DY(){return m_camera_y;};
        void SetCamera2DX(int x){m_camera_x=x;};
        void SetCamera2DY(int y){m_camera_y=y;};

    protected:

        int m_camera_x;
        int m_camera_y;

    private:
};

#endif // CCAMERA2D_H
