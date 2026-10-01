package info.stepanoff.trsis.samples.service;

import info.stepanoff.trsis.samples.db.dao.PropertyRepository;
import info.stepanoff.trsis.samples.db.dao.AuditEventRepository;
import info.stepanoff.trsis.samples.db.model.AuditEventPE;
import info.stepanoff.trsis.samples.db.model.PropertyPE;
import info.stepanoff.trsis.samples.rest.ResourceNotFoundException;
import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import java.util.List;
import java.time.Instant;
import lombok.RequiredArgsConstructor;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
@RequiredArgsConstructor
@Transactional(readOnly = true)
public class PropertyServiceImpl implements PropertyService {

    private final PropertyRepository propertyRepository;
    private final AuditEventRepository auditEventRepository;

    @Override
    public List<PropertyDTO> findAll() {
        return propertyRepository.findAll().stream().map(this::toDto).toList();
    }

    @Override
    public PropertyDTO findById(Long id) {
        return toDto(findEntity(id));
    }

    @Override
    @Transactional
    public PropertyDTO create(PropertyDTO property) {
        PropertyPE entity = new PropertyPE();
        copyFields(property, entity);
        PropertyPE saved = propertyRepository.save(entity);
        audit("CREATE", saved.getId());
        return toDto(saved);
    }

    @Override
    @Transactional
    public PropertyDTO update(Long id, PropertyDTO property) {
        PropertyPE entity = findEntity(id);
        copyFields(property, entity);
        PropertyPE saved = propertyRepository.save(entity);
        audit("UPDATE", saved.getId());
        return toDto(saved);
    }

    @Override
    @Transactional
    public void delete(Long id) {
        PropertyPE entity = findEntity(id);
        propertyRepository.delete(entity);
        audit("DELETE", id);
    }

    private void audit(String action, Long propertyId) {
        AuditEventPE event = new AuditEventPE();
        event.setActor(SecurityContextHolder.getContext().getAuthentication().getName());
        event.setAction(action);
        event.setPropertyId(propertyId);
        event.setOccurredAt(Instant.now());
        auditEventRepository.save(event);
    }

    private PropertyPE findEntity(Long id) {
        return propertyRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Объект недвижимости " + id + " не найден"));
    }

    private void copyFields(PropertyDTO source, PropertyPE target) {
        target.setAddress(source.getAddress());
        target.setType(source.getType());
        target.setArea(source.getArea());
        target.setRooms(source.getRooms());
        target.setMonthlyRent(source.getMonthlyRent());
        target.setAvailable(source.getAvailable());
    }

    private PropertyDTO toDto(PropertyPE entity) {
        PropertyDTO dto = new PropertyDTO();
        dto.setId(entity.getId());
        dto.setAddress(entity.getAddress());
        dto.setType(entity.getType());
        dto.setArea(entity.getArea());
        dto.setRooms(entity.getRooms());
        dto.setMonthlyRent(entity.getMonthlyRent());
        dto.setAvailable(entity.getAvailable());
        return dto;
    }
}
