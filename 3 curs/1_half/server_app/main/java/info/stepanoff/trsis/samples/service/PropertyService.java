package info.stepanoff.trsis.samples.service;

import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import java.util.List;

public interface PropertyService {
    List<PropertyDTO> findAll();
    PropertyDTO findById(Long id);
    PropertyDTO create(PropertyDTO property);
    PropertyDTO update(Long id, PropertyDTO property);
    void delete(Long id);
}
