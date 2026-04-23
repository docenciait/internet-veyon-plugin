#pragma once

#include <QObject>
#include <QProcess>
#include <QVersionNumber>
#include <QWidget>
#include <QVariantMap>
#include <QPointer>

#include "Feature.h"
#include "FeatureProviderInterface.h"
#include "PluginInterface.h"
#include "VeyonMasterInterface.h"

class InternetControlPlugin : public QObject, public FeatureProviderInterface, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.veyon.Veyon.PluginInterface" FILE "InternetControl.json")
    Q_INTERFACES(PluginInterface FeatureProviderInterface)

public:
    explicit InternetControlPlugin(QObject *parent = nullptr);
    ~InternetControlPlugin() override = default;

    Plugin::Uid uid() const override { return Plugin::Uid{QStringLiteral("11111111-2222-3333-4444-555555555555")}; }
    QVersionNumber version() const override { return QVersionNumber(1, 0); }
    QString name() const override { return QStringLiteral("InternetControl"); }
    QString description() const override { return tr("Control de acceso a Internet de los alumnos"); }
    QString vendor() const override { return QStringLiteral("Tu Nombre"); }
    QString copyright() const override { return QStringLiteral("2024"); }

    const FeatureList &featureList() const override;

    bool startFeature(VeyonMasterInterface &master, const Feature &feature,
                      const ComputerControlInterfaceList &computerControlInterfaces) override;

    bool controlFeature(Feature::Uid featureUid, Operation operation, const QVariantMap &arguments,
                        const ComputerControlInterfaceList &computerControlInterfaces) override;

    bool handleFeatureMessage(VeyonServerInterface &server, const MessageContext &messageContext,
                              const FeatureMessage &message) override;

    bool handleFeatureMessage(VeyonWorkerInterface &worker, const FeatureMessage &message) override;

private:
    Feature m_blockFeature;
    Feature m_allowFeature;
    FeatureList m_features;

    QPointer<QWidget> m_banner;

    void showBanner();
    void hideBanner();
};