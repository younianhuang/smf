#ifndef SFMLRENDERWINDOW_H
#define SFMLRENDERWINDOW_H

#include <QWidget>
#include <SFML/Graphics.hpp>
#include "FrameStatistics.h"

class SFMLRenderWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SFMLRenderWindow(QWidget *parent = 0);

    virtual ~SFMLRenderWindow();

    void setFont(sf::Font& font);

    void Update();

    void Stop();



signals:

public slots:

private:

    void OnInit();

    void OnUpdate();

    void ProcessData();

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    virtual void resizeEvent (QResizeEvent * event);    

    private:
        bool   m_Initialized;

        sf::RenderWindow m_RenderWindow;
        sf::RectangleShape m_VideoPane;
        sf::RectangleShape m_RectShape;
        sf::ConvexShape m_ConvexShape;
        sf::Texture m_Texture;
        sf::Image m_Image;
        sf::Text m_Text;
        sf::Font* m_pFont;        
        //sf::Thread m_Thread;
        sf::Mutex m_Mutex;
        FrameStatistics m_FrameStatistics;
        bool m_TextureUpdated;
        bool m_Run;
        float m_FPS;

        bool m_WindowSizeChanged;
        QSize m_WindowSize;
};

#endif // SFMLRENDERWINDOW_H
