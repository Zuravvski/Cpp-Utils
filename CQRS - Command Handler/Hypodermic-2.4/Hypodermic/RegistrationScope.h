#pragma once

#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/RegistrationContext.h"
#include "Hypodermic/ResolutionContainer.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class RegistrationScope : public IRegistrationScope
    {
    public:
        void addRegistration(const std::shared_ptr< IRegistration >& registration) override
        {
            if (registration->typeAliases().empty())
            {
                addRegistration(createKeyForType(registration->instanceType()), registration);
                return;
            }

            for (auto&& x : registration->typeAliases())
                addRegistration(x.first, registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto hasRegistrations = false;
            {
                auto it = m_fallbackRegistrationContextsByBaseTypes.find(typeAliasKey);
                if (it != std::end(m_fallbackRegistrationContextsByBaseTypes))
                {
                    auto& contexts = it->second;
                    std::copy(contexts.rbegin(), contexts.rend(), std::back_inserter(registrationContexts));

                    hasRegistrations = true;
                }
            }

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                return hasRegistrations;

            auto& contexts = it->second;
            std::copy(std::begin(contexts), std::end(contexts), std::back_inserter(registrationContexts));
            return true;
        }

        std::shared_ptr< IRegistrationScope > clone() override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto scopeClone = std::make_shared< RegistrationScope >();
            scopeClone->m_registrationContextsByBaseTypes = m_registrationContextsByBaseTypes;
            scopeClone->m_fallbackRegistrationContextsByBaseTypes = m_fallbackRegistrationContextsByBaseTypes;

            return scopeClone;
        }

    private:
        void addRegistration(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            if (registration->isFallback())
            {
                addRegistration(m_fallbackRegistrationContextsByBaseTypes, typeAliasKey, registration);
            }
            else
            {
                addRegistration(m_registrationContextsByBaseTypes, typeAliasKey, registration);
            }
        }

        template <class TRegistrationContextsByBaseTypes>
        void addRegistration(TRegistrationContextsByBaseTypes& container, const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            auto it = container.find(typeAliasKey);
            if (it == std::end(container))
                it = container.insert(std::make_pair(typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >())).first;

            it->second.push_back(std::make_shared< RegistrationContext >(m_resolutionContainer, registration));
        }

    private:
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_registrationContextsByBaseTypes;
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_fallbackRegistrationContextsByBaseTypes;
        mutable std::recursive_mutex m_mutex;
        ResolutionContainer m_resolutionContainer;
    };

} // namespace Hypodermic