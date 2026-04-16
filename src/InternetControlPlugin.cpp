#include "InternetControlPlugin.h"
#include <QDebug>

InternetControlPlugin::InternetControlPlugin(QObject* parent) :
    QObject(parent),
    m_blockFeature(
        QStringLiteral("BlockInternet"),
        Feature::Flag::Action | Feature::Flag::Master,
        Feature::Uid(QStringLiteral("66666666-7777-8888-9999-000000000000")),
        Feature::Uid(),
        tr("Bloquear Internet"),
        tr("Bloquear"),
        tr("Corta la navegación web de los alumnos seleccionados"),
        QStringLiteral(":/icon_block.svg")
    ),
    m_allowFeature(
        QStringLiteral("AllowInternet"),
        Feature::Flag::Action | Feature::Flag::Master,
        Feature::Uid(QStringLiteral("77777777-7777-8888-9999-111111111111")),
        Feature::Uid(),
        tr("Permitir Internet"),
        tr("Permitir"),
        tr("Restaura la navegación web de los alumnos seleccionados"),
        QStringLiteral(":/icon_allow.svg")
    ),
    m_features({ m_blockFeature, m_allowFeature })
{
}

const FeatureList& InternetControlPlugin::featureList() const
{
    return m_features;
}

// =========================================================================
// PARTE 1: LO QUE OCURRE EN EL ORDENADOR DEL PROFESOR (MASTER)
// =========================================================================
bool InternetControlPlugin::startFeature(VeyonMasterInterface& master, const Feature& feature, 
                                         const ComputerControlInterfaceList& computerControlInterfaces)
{
    // Si la característica es una de las nuestras, enviamos la orden a los alumnos
    if (feature.uid() == m_blockFeature.uid() || feature.uid() == m_allowFeature.uid()) {
        
        // Creamos un mensaje con el ID del botón que se ha pulsado
        FeatureMessage message(feature.uid());
        
        // Iteramos sobre todos los ordenadores que el profesor tenga seleccionados
        for (auto computer : computerControlInterfaces) {
            // Le disparamos el mensaje por la red a cada alumno
        computer->sendFeatureMessage(message);        }
        
        return true;
    }
    
    return false;
}

// =========================================================================
// PARTE 2: LO QUE OCURRE EN EL ORDENADOR DEL ALUMNO (WORKER/SERVER)
// =========================================================================
bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface& worker, const FeatureMessage& message)
{
    // Si el alumno recibe el mensaje de BLOQUEAR
    if (message.featureUid() == m_blockFeature.uid()) {
        // Bloqueamos tráfico HTTP (80) y HTTPS (443)
        // Usamos -I para insertarlo al principio de las reglas para que tenga prioridad
        QProcess::execute("iptables", {"-I", "OUTPUT", "-p", "tcp", "--dport", "80", "-j", "DROP"});
        QProcess::execute("iptables", {"-I", "OUTPUT", "-p", "tcp", "--dport", "443", "-j", "DROP"});
        return true;
    }
    
    // Si el alumno recibe el mensaje de PERMITIR
    if (message.featureUid() == m_allowFeature.uid()) {
        // Borramos las reglas exactas que creamos antes
        // Usamos -D para eliminarlas
        QProcess::execute("iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "80", "-j", "DROP"});
        QProcess::execute("iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "443", "-j", "DROP"});
        return true;
    }

    return false;
}

// Funciones no utilizadas pero obligatorias por la interfaz
bool InternetControlPlugin::controlFeature(Feature::Uid featureUid, Operation operation, const QVariantMap& arguments, const ComputerControlInterfaceList& computerControlInterfaces) { return false; }
bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface& server, const MessageContext& messageContext, const FeatureMessage& message) { return false; }