#ifndef GLVR_WIDGET_H
#define GLVR_WIDGET_H


#include <QWidget>
#include <QImage>
#include <GLVR.h>
#include <QPainter>


template<class QWidgetType>
class GLVRWidget : public  QWidgetType
{
public:
    template<class... Args>
    explicit GLVRWidget(surveon::glvr::IVideoRenderer* pVideoRenderer,  Args&&... arg);

    virtual ~GLVRWidget()   {}

    void showWidget();

    void hideWidget();

    void setWidgetOpacity(float opacity);

protected:
    virtual QPaintEngine* paintEngine() const;

    virtual void closeEvent ( QCloseEvent * event );
    virtual void moveEvent(QMoveEvent * event);
    virtual void paintEvent(QPaintEvent*);
    virtual void changeEvent(QEvent * event);

    void createGLVRFResouces(void);
    void renderToTexture(void);

private:
    surveon::glvr::IVideoRenderer* m_pVideoRenderer;
    surveon::glvr::ISprite* m_pSprite;
    surveon::glvr::ITexture* m_pTexture;
    std::unique_ptr<QImage> m_pImage;
    bool m_RenderToTexture;
};

//=======================================================================================
template<class QWidgetType>
template<class... Args>
GLVRWidget<QWidgetType>::GLVRWidget(surveon::glvr::IVideoRenderer* pVideoRenderer,  Args&&... arg) :
    QWidgetType(arg...),
    m_pVideoRenderer(pVideoRenderer),
    m_pSprite(nullptr),
    m_pTexture(nullptr),
    m_pImage(nullptr),
    m_RenderToTexture(false)
{

}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::closeEvent(QCloseEvent *)
{
    if(m_pSprite)
        m_pVideoRenderer->destroySprite(m_pSprite);

    if(m_pTexture)
        surveon::glvr::VideoRendererManager::getInstance().destroyTexture(m_pTexture);

    m_pImage.reset();
}


template<class QWidgetType>
void GLVRWidget<QWidgetType>::moveEvent(QMoveEvent * event)
{
    QWidgetType::moveEvent(event);

    if(m_pSprite)
    {
        QRect rect = this->frameGeometry();
        m_pSprite->setRect(rect.left(), rect.top(),  rect.right(), rect.bottom());
    }    
}

template<class QWidgetType>
QPaintEngine* GLVRWidget<QWidgetType>::paintEngine() const
{
    return 0;
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::paintEvent(QPaintEvent* event)
{
    QWidgetType::paintEvent(event);

    if(!m_RenderToTexture) // prevent recursive
    {
        renderToTexture();
    }
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::changeEvent(QEvent *)
{

}



template<class QWidgetType>
void GLVRWidget<QWidgetType>::createGLVRFResouces(void)
{
    QRect rect = this->frameGeometry();

    m_pImage.reset(new QImage(rect.width(), rect.height(), QImage::Format_RGB32));

    m_pTexture = surveon::glvr::VideoRendererManager::getInstancePtr()->createTexture();
    m_pTexture->create(rect.width(), rect.height(), surveon::glvr::ITexture::PF_RGBA);

    m_pSprite = m_pVideoRenderer->createSprite(rect.left(), rect.top(),  rect.right() , rect.bottom());
    m_pSprite->setTexture(m_pTexture);
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::renderToTexture(void)
{
    if(m_pTexture && m_pImage.get())
    {
        m_RenderToTexture = true;

        this->render(m_pImage.get());
        m_pTexture->upload(m_pImage->bits(), surveon::glvr::ITexture::PF_BGRA);

        m_RenderToTexture = false;
    }
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::showWidget()
{
    // show Widget
    this->setAttribute(Qt::WA_DontShowOnScreen, true); // hide Qt painting
    this->show();
    this->setAttribute(Qt::WA_DontShowOnScreen, false); // let callback events work

    // show GLVR Sprite
    if(!m_pSprite)
        createGLVRFResouces();

    m_pSprite->show();
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::hideWidget()
{
    this->hide();

    if(m_pSprite)
        m_pSprite->hide();
}

template<class QWidgetType>
void GLVRWidget<QWidgetType>::setWidgetOpacity(float opacity)
{
    if(m_pSprite)
        m_pSprite->setColor(1.0f, 1.0f,1.0f, opacity);
}

#endif // GLVR_WIDGET_H
