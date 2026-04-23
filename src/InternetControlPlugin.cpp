#include "InternetControlPlugin.h"
#include <QProcess>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QScreen>

#include "VeyonServerInterface.h"
#include "VeyonMasterInterface.h"
#include "ComputerControlInterface.h"
#include "VeyonCore.h"
#include "Computer.h"
#include "Feature.h"

InternetControlPlugin::InternetControlPlugin(QObject *parent)
    : QObject(parent),
      m_blockFeature(
          QStringLiteral("BlockInternet"),
          Feature::Flag::Action | Feature::Flag::Master,
          Feature::Uid(QStringLiteral("66666666-7777-8888-9999-000000000000")),
          Feature::Uid(),
          tr("Bloquear Internet"), tr("Bloquear"), tr("Corta la navegación"),
          QStringLiteral(":/icon_block.svg")),
      m_allowFeature(
          QStringLiteral("AllowInternet"),
          Feature::Flag::Action | Feature::Flag::Master,
          Feature::Uid(QStringLiteral("77777777-7777-8888-9999-111111111111")),
          Feature::Uid(),
          tr("Permitir Internet"), tr("Permitir"), tr("Restaura la navegación"),
          QStringLiteral(":/icon_allow.svg")),
      m_features({m_blockFeature, m_allowFeature})
{
}

const FeatureList &InternetControlPlugin::featureList() const
{
    return m_features;
}

bool InternetControlPlugin::startFeature(VeyonMasterInterface &master,
                                         const Feature &feature,
                                         const ComputerControlInterfaceList &computerControlInterfaces)
{
    if (computerControlInterfaces.isEmpty())
        return false;

    FeatureMessage message(feature.uid());

    if (feature.uid() == m_blockFeature.uid())
        message.addArgument(0, true);
    else if (feature.uid() == m_allowFeature.uid())
        message.addArgument(0, false);

    for (auto computerControl : computerControlInterfaces)
        computerControl->sendFeatureMessage(message);

    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface &server,
                                                 const MessageContext &messageContext,
                                                 const FeatureMessage &message)
{
    if (message.featureUid() == m_blockFeature.uid())
    {
        // BLOQUEO INTERNET
        QProcess::execute("iptables", {"-F"});
        QProcess::execute("iptables", {"-X"});
        QProcess::execute("iptables", {"-P", "INPUT", "DROP"});
        QProcess::execute("iptables", {"-P", "FORWARD", "DROP"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "DROP"});

        QProcess::execute("iptables", {"-A", "INPUT", "-i", "lo", "-j", "ACCEPT"});
        QProcess::execute("iptables", {"-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"});

        const QStringList profes = {"192.168.50.2", "192.168.50.50", "192.168.50.100", "192.168.50.11"};
        for (const QString &ip : profes)
        {
            QProcess::execute("iptables", {"-A", "OUTPUT", "-d", ip, "-j", "ACCEPT"});
            QProcess::execute("iptables", {"-A", "INPUT", "-s", ip, "-m", "state", "--state", "NEW,ESTABLISHED", "-j", "ACCEPT"});
        }

        // 🔴 MOSTRAR BANNER QT
        showBanner();

        FeatureMessage reply(message.featureUid());
        reply.addArgument(0, true);
        server.sendFeatureMessageReply(messageContext, reply);

        return true;
    }
    else if (message.featureUid() == m_allowFeature.uid())
    {
        // RESTAURAR INTERNET
        QProcess::execute("iptables", {"-P", "INPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "FORWARD", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-F"});

        // 🔴 OCULTAR BANNER
        hideBanner();

        FeatureMessage reply(message.featureUid());
        reply.addArgument(0, false);
        server.sendFeatureMessageReply(messageContext, reply);

        return true;
    }

    return false;
}

bool InternetControlPlugin::controlFeature(
    Feature::Uid,
    Operation,
    const QVariantMap &,
    const ComputerControlInterfaceList &)
{
    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface &, const FeatureMessage &)
{
    return false;
}

//
// 🔴 BANNER (SIN ROMPER NADA)
//

void InternetControlPlugin::showBanner()
{
    if (m_banner)
        return;

    m_banner = new QWidget();
    m_banner->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    m_banner->setStyleSheet("background-color: rgba(200,0,0,200);");

    QLabel *label = new QLabel("NO INTERNET");
    label->setStyleSheet("color: white; font-size: 28px; font-weight: bold;");
    label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout(m_banner);
    layout->addWidget(label);

    QRect screen = QGuiApplication::primaryScreen()->geometry();

    // banner arriba
    m_banner->setGeometry(0, 0, screen.width(), 80);

    m_banner->show();
}

void InternetControlPlugin::hideBanner()
{
    if (m_banner)
    {
        m_banner->close();
        delete m_banner;
        m_banner = nullptr;
    }
}